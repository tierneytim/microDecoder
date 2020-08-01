
#include "SD.h"                               // input
#include "mp3.h"                              // decoder

// needs I2S32 library (https://github.com/tierneytim/I2S32)
#include "I2S32.h"                              // output

mp3 MP3;                                      // MP3 class
pcm audio;                                    // Generic audio data class 
I2S i2s;                                      // I2S class used for output
 
void setup() {
  SD.begin();                                 // Start SD interface
  File file = SD.open("/cc.mp3");           // Open the desired file
  
  MP3.begin(file);                            // pass the file to the MP3 Class
  
  // bclk, din, wsel
  i2s.begin(26,25,27);                        // Set pins for I2S 
  i2s.setSampleRate(MP3.Fs);                  // Set sample rate
}

void loop() {
  audio = MP3.decode();                	      // decode some data 
  i2s.write(audio.interleaved,audio.nBytes);  // send it to I2S buffer
}