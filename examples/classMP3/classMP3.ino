#include "SD_MMC.h" // input
#include "mp3.h"    // decoder

mp3 MP3;

void setup() {
  Serial.begin(115200);
  
  SD_MMC.begin("/sdcard",true);
  File file = SD_MMC.open("/cc.mp3");
  
  MP3.begin(file);
  Serial.print("Bits per Sample: ");
  Serial.println(MP3.bitsPerSample);
  Serial.print("Sample Rate: ");
  Serial.println(MP3.Fs);
}

void loop() {
}