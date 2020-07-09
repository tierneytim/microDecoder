#ifndef PCM_H 
#define PCM_H
#include "Arduino.h"

class pcm {
	public:
		void setSampleNumber(int n){
			nSamps = n;
			nBytes = n*4;
			delete[] interleaved;
			interleaved = new int16_t[n*2];
		}
		void releaseMemory(){
			delete[] interleaved;
		}
		unsigned int Fs;
		int bitsPerSample=16;
		int numChannels=2;
		int nSamps=32;
		int16_t *interleaved = new int16_t[32*2];
        int nBytes = 32*4;
		
};
#endif