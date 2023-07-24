int latchPin = 8;    //Pin connected to ST_CP of 74HC595(12)
int clockPin = 12;   //Pin connected to SH_CP of 74HC595(11)
int dataPin = 11;    //Pin connected to DS of 74HC595(14)
//
void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
// count from 0 to 255 and display the number
// on the LEDs
  static int numberToDisplay = 0 ; // 0 3 7 15 0 128 192 224 11101000(reverse)
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay); // shift out the bits:
  digitalWrite(latchPin, HIGH); //take the latch pin high so the LEDs will light up:
  delay(1000); // pause before next value: 
}









//for (int numberToDisplay = 15; numberToDisplay>=1; numberToDisplay = int((numberToDisplay-1)/2)) {
