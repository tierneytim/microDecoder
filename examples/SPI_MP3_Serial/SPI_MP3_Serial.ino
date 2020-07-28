#include "SD.h"                                   // input
#include "mp3.h"                                  // decoder

mp3 MP3;                                          // MP3 Class
pcm audio;                                        // raw audio data

void setup() {
  Serial.begin(115200);                           // Setup Serial
  SD.begin();                                     // Setup SD connection
 
  File file = SD.open("/cc.mp3");                 // Open an MP3 File
  
  MP3.begin(file);                                // Pass file to MP3 class
}

void loop() {
  audio = MP3.decode();                          // Decode audio data into pcm class
  
  /* there are 32 samples in audio.interleaved (16 left and 16 right)
   *  but we are only going to plot the first data point in each channel.
   *  This effectively downamples the data by  a factor of 16 (for
   *   viewing the waveform only)
   */
  
  Serial.print(audio.interleaved[0]);            // left channel
  Serial.print(" ");                      
  Serial.println(audio.interleaved[1]);         // right channel  
}