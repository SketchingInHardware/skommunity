uint8_t myVoice;
const int voiceSensor = 0;  //A0 input

//20*5*number
//20Hz - 655Hz

void readVoice()
{
  int val = analogRead(voiceSensor);
  myVoice = (uint8_t) (map(val, 0, 1023, 0, 127));
}
