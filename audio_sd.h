/*
 To convert files for use with this syster use this on Linux:

 sox inputfile.wav --norm=-1 -e unsigned-integer -b 8 -r 31250 -c 1 -t raw outputfile.raw

 For more info:
 http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio
 
 */
#include <SimpleSDAudio.h>

#define SD_CS_PIN A5
#define SSDA_INIT_OPTIONS (SSDA_MODE_FULLRATE | SSDA_MODE_MONO_BRIDGE) // | SSDA_MODE_AUTOWORKER)
#define AUDIO_FILENAME "EXAMPLE.AFM"
#define AUDIO_BUFFER_SIZE 1024

#include <avr/wdt.h>
  
void playAudio()
{
  Serial.print(F("Audio:I:"));  
  uint8_t audioBuffer[AUDIO_BUFFER_SIZE];
  SdPlay.setSDCSPin(SD_CS_PIN);
  SdPlay.setWorkBuffer(audioBuffer, AUDIO_BUFFER_SIZE);
  
  if (!SdPlay.init(SSDA_INIT_OPTIONS)) {
    Serial.print(F("E:"));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("OK")); 
  }

  Serial.print(F("Audio:F:"));
  if(!SdPlay.setFile(AUDIO_FILENAME)) {
    Serial.println(F("E: "));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("OK")); 
  }

  SdPlay.play();
  while (!SdPlay.isStopped()) SdPlay.worker();

  // Audio playback leaves system in a hung state.
  // Instead of trying to fix this now, just reboot to a clean state.
  // TODO: close down all other resources gracefully before playback.
  Serial.println(F("Done:Rebooting..."));
  wdt_enable(WDTO_15MS);
  while (true) {}
}



