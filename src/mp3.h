#ifndef MP3_H
#define MP3_H

#include "Arduino.h"
#include "libmad/config.h"
#include "libmad/mad.h"
#include "FS.h"
#include "pcm.h"

class mp3 {
	public:
		void begin(File file);
		void end();
		pcm decode();
		void getMetadata();
		unsigned int Fs;
		int bitsPerSample;
		 
	private:
		pcm _audio;
		int _chunkNumber=37;
		int _maxChunkNumber =36;
		int _sampleNumber = 9999;
		int _lastBuffLen;
	    File _file;
		struct mad_stream *_stream;
		struct mad_frame *_frame;
		struct mad_synth *_synth;
		int bufSize = 1152;
		unsigned char* _mp3Buf; 
		
		

};
#endif