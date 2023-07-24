int ledPin1 = 9;   // LED1 output pin
int ledPin2 = 10;  // LED2 output pin
int pirPin1 = 2;   // PIR1 input pin
int pirPin2 = 3;   // PIR2 input pin
int serialIn = 4;  //serial data input pin
int clk = 5, b1, b2;// clks and brightness
int latchPin = 8;    //Pin connected to ST_CP of 74HC595
int clockPin = 12;   //Pin connected to SH_CP of 74HC595
int dataPin = 11;    //Pin connected to DS of 74HC595
int binaryNumber1  = 0;
int binaryNumber2  = 0;
unsigned long  decimalNumber = 0;
int array[8] = {0};
unsigned long currentT1 = 0, currentT2 = 0, k = 0;
bool pir1, pir2;
String brightValues[2];
int b_1[2], b_2[2];

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
  pinMode(serialIn, OUTPUT);
  pinMode(clk, OUTPUT);

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  Serial.begin(19200);
}

void CalcDelay(unsigned long currentT1, unsigned long currentT2, unsigned long delay1, unsigned long delay2, bool start, unsigned long k, int b_1[], int b_2[]) {
  int bit1, bit2;
  String str_bit2, str_bit1;
  Serial.println(k);
  Serial.println(start);
  Serial.print("time 1: ");
  Serial.println(currentT1);
  Serial.print("delay1 :");
  Serial.println(delay1);
  Serial.print("time 2: ");
  Serial.println(currentT2);
  Serial.print("delay2 :");
  Serial.println(delay2);
  
  if(k ==0 ){
    bit1 = 0; bit2 = 0;
  }
  if (start) {
    
    if (delay1 > delay2) {
      b1 = 4;
      b2 = 0;
    }
    else if (delay2 > delay1) {
      b1 = 0;
      b2 = 4;
    }
 
  }
  else {
    if (delay1 > delay2) {
      if (delay2 == 0) delay2 = 1;
      Serial.println("hey...delay1>delay2");
      b1 = b1 + 2;
      b2 = b2 - (int)(((float)delay2 / delay1) * 4);
      if (b1 > 4) {
        b1 = 4;
      }
      if (b2 < 0) {
        b2 = 0;
      }
    }
    else if (delay2 > delay1) {
      if (delay1 == 0) delay1 = 1;
      Serial.println("hey...delay2>delay1");
      b2 = b2 + 2;
      b1 = b1 - (int)(((float)delay1 / delay2) * 4);
      if (b2 > 4) {
        b2 = 4;
      }
      if (b1 < 0) {
        b1 = 0;
      }
    }
  }
  
  //Adding values to b_1[], b_2[] arrays

  if(k<2){
    b_1[k]  = b1;
    b_2[k]  = b2;
    bit1 = b1;
    bit2 = b2;
  }
  else{
    b_2[k%2]  = b2;
    b_1[k%2]  = b1;
    int sum_1 = 0, sum_2=0;
    for(int i=0;i<2;i++){
      sum_1+=b_1[i];
      sum_2+=b_2[i];
    }
    bit1 = int(sum_1/2);
    bit2 = int(sum_2/2);
  }
  Serial.print("bit1 :");
  Serial.println(bit1);
  Serial.print("bit2 :");
  Serial.println(bit2); 
  
  if(bit2 == 0 || bit2 == 1) bit2 = 0;
  else if(bit2 == 2) bit2 = 1;
  else if(bit2 == 3) bit2 = 3;
  else bit2 = 7;

  if(bit1 == 0 || bit1 == 1) bit1 = 0;
  else if(bit1 == 2) bit1 = 1;
  else if(bit1 == 3) bit1 = 3;
  else bit1 = 7;

  
  binaryNumber1  = bit1 & 0b00001111;
  binaryNumber2  = bit2 & 0b00001111;
  
  for (int i = 0; i < 4; i++) {
    array[i] = (binaryNumber1 >> i) & 1;
    array[4+i] = (binaryNumber2 >> i) & 1;
  }
  for (int i = 7; i >= 0; i--) {
      Serial.print(array[i]);
  } 
  Serial.println();
  
  for (int i = 7; i >= 0; i--) {
    decimalNumber = (decimalNumber << 1) | array[i];
  } 
  
  Serial.println(decimalNumber);
  Serial.println();
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, decimalNumber); // shift out the bits:// bit1 in MSB
  digitalWrite(latchPin, HIGH); //take the latch pin high so the LEDs will light up:
  }
  
void loop() {
  pir1 = digitalRead(pirPin1);
  pir2 = digitalRead(pirPin2);
  static unsigned long previous_t1 = 0, previous_t2 = 0, delay1 = 0, delay2 = 0;
  bool enter = false, start = false;
  
  // put your main code here, to run repeatedly:
  if (pir1 == 1) {
    currentT1 = (millis() / 1000);
    enter = true;
  }
  if (pir2 == 1) {
    currentT2 = (millis() / 1000);
    enter = true;
  }

  if (previous_t1 == 0 && previous_t2 == 0 && enter) {
    start = true;
  }

  if(k == 0){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, decimalNumber); // shift out the bits:// bit1 in MSB
    digitalWrite(latchPin, HIGH); //take the latch pin high so the LEDs will light up:
//    Serial.println(decimalNumber); 
  }
  
  if (previous_t1 != currentT1 || previous_t2 != currentT2) {
    delay1 = currentT1 - previous_t1;
    delay2 = currentT2 - previous_t2;
    previous_t1 = currentT1;
    previous_t2 = currentT2;
    CalcDelay(currentT1, currentT2, delay1, delay2, start, k, b_1, b_2 );
    delay(3000);
    k++;
  }
}
