const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
 
const int audioPin = 10;

void beep(int note, int duration)
{
  tone(audioPin, note, duration);
  delay(duration);
  noTone(audioPin);
  delay(50);
}

#define MELODY_LEN 34
 
// This blocks every other function of the device while audio is playing
void playAudio()
{
  int notes[MELODY_LEN] = {a,a,a,f,cH,a,f,cH,a,eH,eH,eH,fH,cH,gS,f,cH,a,aH,a,a,aH,gSH,gH,fSH,fH,fSH,aS,dSH,dH,cSH,cH,b,cH};
  int durations[MELODY_LEN] = {500,500,500,350,150,500,350,150,650,500,500,500,350,150,500,350,150,650,500,300,150,500,325,175,125,125,250,250,500,325,175,125,125,250};

  pinMode(audioPin, OUTPUT);
  for (uint8_t i = 0; i < MELODY_LEN; i++) {
    beep(notes[i], durations[1]);
  }
}

