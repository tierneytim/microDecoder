#ifndef WAV_H
#define WAV_H

#include "Arduino.h"
#include "SD.h"
#include "pcm.h"

class wav {
	public:
		void begin(File file);
		void end();
		pcm decode();
		void getMetadata();
		unsigned int Fs = 0;
		int bitsPerSample;
		 
	
	private:	
		pcm _audio;
	    File _file;
};
#endif