#include "wav.h"

void wav::begin(File file){
	_file = file;
	_audio.setSampleNumber(_audio.nSamps);
}

pcm wav::decode(){

int retval = _file.read((uint8_t*)_audio.interleaved, _audio.nBytes);

if (retval <= 0) {
	memset(_audio.interleaved,0,_audio.nBytes);
    return(_audio);
}

// Zero-fill a short final read so stale samples aren't replayed
if (retval < _audio.nBytes) {
	memset((uint8_t*)_audio.interleaved + retval, 0, _audio.nBytes - retval);
}
	
return _audio;
		
}

void wav::getMetadata(){
	unsigned long currentPosition=_file.position();
	_file.seek(0);
	
	uint8_t header[44];
	_file.read(header,44);
	char RIFF[4];
    for(int i=0;i<4;i++){
		Serial.print(char(header[i]));
		
	}
	Serial.println("");
	
    uint32_t* ChunkSize = (uint32_t*)(&header[4]);
	Serial.print("Chunk Size: ");
	Serial.println(*ChunkSize);
	
    for(int i=8;i<12;i++){
		Serial.print(char(header[i]));	
	}
	Serial.println("");
	
	for(int i=12;i<16;i++){
		Serial.print(char(header[i]));	
	}
	Serial.println("");
	
	uint32_t* Subchunk1Size = (uint32_t*)(&header[16]);
	Serial.print("Subchunk1Size Size: ");
	Serial.println(*Subchunk1Size);
	
	uint16_t* AudioFormat = (uint16_t*)(&header[20]);
	Serial.print("Audio Format: ");
	Serial.println(*AudioFormat);
	
	uint16_t* NumOfChan = (uint16_t*)(&header[22]);
	Serial.print("Number of Channels: ");
	Serial.println(*NumOfChan);
	
	uint32_t* SampleRate = (uint32_t*)(&header[24]);
	Serial.print("Sampling Rate: ");
	Serial.println(*SampleRate);
	_audio.Fs = *SampleRate;
	Fs=*SampleRate;
	
	uint32_t* BPS = (uint32_t*)(&header[28]);
	Serial.print("Bytes per Second: ");
	Serial.println(*BPS);
	
	uint16_t* ba = (uint16_t*)(&header[32]);
	Serial.print("Block Allign: ");
	Serial.println(*ba);
	
	uint16_t* bps = (uint16_t*)(&header[34]);
	Serial.print("bits per sample: ");
	Serial.println(*bps);
    _audio.bitsPerSample=*bps;
	bitsPerSample=*bps;
	_file.seek(currentPosition);
	
}

void wav::end(){
	_audio.releaseMemory();
}
