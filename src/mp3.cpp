#include "mp3.h"

void mp3::begin(File file){
	_file = file;
	
	_stream = reinterpret_cast<struct mad_stream *>(malloc(sizeof(struct mad_stream)));
	_frame = reinterpret_cast<struct mad_frame *>(malloc(sizeof(struct mad_frame)));
	_synth = reinterpret_cast<struct mad_synth *>(malloc(sizeof(struct mad_synth)));
	_mp3Buf = reinterpret_cast<unsigned char *>(malloc(bufSize));

	mad_stream_init(_stream);
	mad_frame_init(_frame);
	mad_synth_init(_synth);
	_audio.setSampleNumber(32);
	
	getMetadata();
}

pcm mp3::decode(){

int keep=0;
bool notDecoded =true;
while(notDecoded){
 if (_chunkNumber >= _maxChunkNumber && _sampleNumber>=32) {
		
  if (_stream->next_frame != NULL) {
   keep = _lastBuffLen - (_stream->next_frame - _mp3Buf);
   memmove(_mp3Buf, _stream->next_frame, keep);
   _stream->next_frame = NULL;
  }
      
  int retval = _file.read(_mp3Buf+keep, bufSize - keep);
		
  if (retval < 0) {
   memset(_audio.interleaved,0,128);
   _audio.nSamps=32;	
   return(_audio);
  }
		
		
  if (retval == 0) {
   if (keep + MAD_BUFFER_GUARD <= bufSize) {
    memset(_mp3Buf + keep, 0, MAD_BUFFER_GUARD);
	_lastBuffLen = keep + MAD_BUFFER_GUARD;
	continue;
   }else {
    _lastBuffLen = 0;
   }
  }
		
  if (retval > 0){
   _lastBuffLen = keep + retval;
  }
		
  mad_stream_buffer(_stream, _mp3Buf, _lastBuffLen);
		
  if(mad_frame_decode(_frame, _stream)==-1){
   _chunkNumber++;
   continue;
  }else{
   notDecoded=false;
  }
  _chunkNumber=0;
 }
 break;
}

mad_synth_frame_onens(_synth, _frame, _chunkNumber++);

int i =0;
int j =0;
int k =0;
while (i<32) { 
 _audio.interleaved[k++] = _synth->pcm.samples[0][i++]; 
 _audio.interleaved[k++] = _synth->pcm.samples[1][j++]; 
 } 

_audio.nSamps=32;
_audio.Fs = _synth->pcm.samplerate;
	
return _audio;
		
}
void mp3::getMetadata(){
	unsigned long currentPosition=_file.position();
	_file.seek(0);
	_audio.bitsPerSample=16;
	bitsPerSample=16;
	while(Fs==0){
		decode();
		Fs=_audio.Fs;
	}
	
	_file.seek(currentPosition);
	
}

void mp3::end(){
	_file.close();
	mad_synth_finish(_synth);
	mad_frame_finish(_frame);
	mad_stream_finish(_stream);
	free(_mp3Buf); 
	_audio.releaseMemory();
}
