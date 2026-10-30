#ifndef PCM_H
#define PCM_H
#include "Arduino.h"

class pcm {
	public:
		// Fixed capacity of interleaved, in sample frames (one sample per channel)
		static const int maxSamples = 32;
        int16_t *interleaved = new int16_t[maxSamples*2];

		void setSampleNumber(int n){
			// interleaved is statically sized, so never report more than it holds
			if (n > maxSamples) n = maxSamples;
			nSamps = n;
			nBytes = n * numChannels * (bitsPerSample / 8);
		}
		void releaseMemory(){
			//delete[] interleaved;
		}
		unsigned int Fs = 0;
		int bitsPerSample=16;
		int numChannels=2;
		int nSamps=32;

		// Bytes in one block: nSamps * numChannels * bytes per sample
        int nBytes = 32 * 2 * 2;

};
#endif