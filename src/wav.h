#ifndef MP3_H
#define MP3_H

#include "Arduino.h"
#include "FS.h"
#include "pcm.h"

class wav {
	public:
		void begin(File file);
		void end();
		pcm decode();
		void getMetadata();
		unsigned int Fs;
		int bitsPerSample;
		 
	
	private:	
		pcm _audio;
	    File _file;
		int bufSize = 32;
		unsigned char* _Buf; 
};
#endif