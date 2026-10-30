#include "mp3.h"

void mp3::begin(File file){
	_file = file;
	
	//_stream = reinterpret_cast<struct mad_stream *>(malloc(sizeof(struct mad_stream)));
	//_frame = reinterpret_cast<struct mad_frame *>(malloc(sizeof(struct mad_frame)));
	//_synth = reinterpret_cast<struct mad_synth *>(malloc(sizeof(struct mad_synth)));
	//_mp3Buf = reinterpret_cast<unsigned char *>(malloc(bufSize));
    
	mad_stream_init(_stream);
	mad_frame_init(_frame);
	mad_synth_init(_synth);
	_eofPadded = false;
	_audio.setSampleNumber(32);
	_dataStart = skipID3();
	Fs = 0; // force getMetadata() to probe this file, not reuse the last one's rate
	getMetadata();
}

// Returns the offset of the first byte after an ID3v2 tag (0 if none). Embedded
// album art is full of 0xFF bytes that look like frame headers to libmad.
unsigned long mp3::skipID3(){
	uint8_t h[10];
	_file.seek(0);
	if (_file.read(h, 10) != 10 || h[0] != 'I' || h[1] != 'D' || h[2] != '3') {
		_file.seek(0);
		return 0;
	}
	// Tag size is a 28-bit syncsafe integer, excluding the 10-byte header (and footer if flagged)
	unsigned long size = ((unsigned long)(h[6] & 0x7F) << 21) | ((unsigned long)(h[7] & 0x7F) << 14) | ((h[8] & 0x7F) << 7) | (h[9] & 0x7F);
	size += (h[5] & 0x10) ? 20 : 10;
	_file.seek(size);
	return size;
}

pcm mp3::decode(){

int keep=0;
bool notDecoded =true;
while(notDecoded){

 if (_chunkNumber >= _maxChunkNumber && _sampleNumber>=32) {
		
  if (_stream->next_frame != NULL) {
   unsigned char const* from = _stream->next_frame;
   keep = _lastBuffLen - (from - _mp3Buf);
   // A bogus header claiming a frame larger than the buffer would leave it full forever: skip a byte to resync
   if (keep >= bufSize) {
    from++;
    keep--;
   }
   memmove(_mp3Buf, from, keep);
   _stream->next_frame = NULL;
  }
  yield();
  int retval = _file.read(_mp3Buf+keep, bufSize - keep);
  yield();
  if (retval < 0) {
   memset(_audio.interleaved,0,_audio.nBytes);
   return(_audio);
  }
		
		
  if (retval == 0) {
   // EOF: pad once with MAD_BUFFER_GUARD zeros so libmad can decode the last frame
   if (!_eofPadded && keep + MAD_BUFFER_GUARD <= bufSize) {
    memset(_mp3Buf + keep, 0, MAD_BUFFER_GUARD);
    keep += MAD_BUFFER_GUARD;
    _eofPadded = true;
   }
   _lastBuffLen = keep;
  }
		
  if (retval > 0){
   _lastBuffLen = keep + retval;
  }
		
  mad_stream_buffer(_stream, _mp3Buf, _lastBuffLen);

  int result;
  // Recoverable errors (lost sync, bad frame) skip ahead within the buffer, so retry before refilling
  while ((result = mad_frame_decode(_frame, _stream)) == -1 && MAD_RECOVERABLE(_stream->error)) {
   yield();
  }
  if(result==-1){
   // No more data will arrive at EOF, so return silence instead of retrying forever
   if (retval == 0) {
    memset(_audio.interleaved,0,_audio.nBytes);
    return(_audio);
   }
   _chunkNumber++;
   continue;
  }else{
   notDecoded=false;
  }
  // 36 chunks of 32 samples for MPEG-1, 18 for MPEG-2/2.5
  _maxChunkNumber = MAD_NSBSAMPLES(&_frame->header);
  _chunkNumber=0;
 }
 break;
}

mad_synth_frame_onens(_synth, _frame, _chunkNumber++);

uint32_t i =0;
uint32_t j =0;
uint32_t k =0;
while (i<32) { 
 _audio.interleaved[k++] = _synth->pcm.samples[0][i++]; 
 _audio.interleaved[k++] = _synth->pcm.samples[1][j++]; 
 } 

_audio.Fs = _synth->pcm.samplerate;
	
return _audio;
		
}
void mp3::getMetadata(){
	unsigned long currentPosition=_file.position();
	if (currentPosition < _dataStart) currentPosition = _dataStart;
	_file.seek(_dataStart);
	_audio.bitsPerSample=16;
	bitsPerSample=16;
	while(Fs==0){
		decode();
		Fs=_audio.Fs;
	}
	
	_file.seek(currentPosition);

	// Discard frames buffered while probing, otherwise they are played twice
	mad_stream_finish(_stream);
	mad_frame_finish(_frame);
	mad_stream_init(_stream);
	mad_frame_init(_frame);
	mad_synth_init(_synth);
	_chunkNumber = _maxChunkNumber;
	_eofPadded = false;

}

void mp3::end(){
	_file.close();
	mad_synth_finish(_synth);
	mad_frame_finish(_frame);
	mad_stream_finish(_stream);
	//free(_mp3Buf); 
	//_audio.releaseMemory();
}
