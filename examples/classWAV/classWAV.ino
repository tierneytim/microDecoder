#include "SD_MMC.h" // input
#include "wav.h"    // decoder

wav WAV;

void setup() {
  Serial.begin(115200);
  
  SD_MMC.begin("/sdcard",true);
  File file = SD_MMC.open("/cc.wav");
  
  WAV.begin(file);
  Serial.print("Bits per Sample: ");
  Serial.println(WAV.bitsPerSample);
  Serial.print("Sample Rate: ");
  Serial.println(WAV.Fs);
}

void loop() {
}


