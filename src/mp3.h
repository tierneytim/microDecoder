#ifndef MP3_H
#define MP3_H

#include "Arduino.h"
#include "libmad/config.h"
#include "libmad/mad.h"
#include "SD.h"
#include "pcm.h"

class mp3 {
	public:
		void begin(File file);
		void end();
		pcm decode();
		void getMetadata();
		unsigned int Fs = 0;
		int bitsPerSample;
		 
	private:
		pcm _audio;
		int _chunkNumber=37;
		int _maxChunkNumber =36;
		int _sampleNumber = 9999;
		int _lastBuffLen;
		bool _eofPadded = false;
	    File _file;
		struct mad_stream *_stream = reinterpret_cast<struct mad_stream *>(malloc(sizeof(struct mad_stream)));
		struct mad_frame *_frame = reinterpret_cast<struct mad_frame *>(malloc(sizeof(struct mad_frame)));
		struct mad_synth *_synth = reinterpret_cast<struct mad_synth *>(malloc(sizeof(struct mad_synth)));
		// Must hold a whole frame: up to 1441 bytes (320kbps @ 32kHz) + MAD_BUFFER_GUARD
		int bufSize = 2048;
		unsigned char* _mp3Buf = reinterpret_cast<unsigned char *>(malloc(bufSize));; 
		
		

};
#endif