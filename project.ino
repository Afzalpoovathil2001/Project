  #include<LiquidCrystal.h>
  #include <Keypad.h>
  #include <math.h>
  LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

  int timer1;
  int timer2;

  float Time;

  int flag1 = 0;
  int flag2 = 0;

  float distance = 20;
  float speed;

  const byte npulse1 = 13; 
  const byte npulse2 = 13; 
  const byte pin_pulse1 = A0; 
  const byte pin_cap1  = A1;
  const byte pin_pulse2 = A3; 
  const byte pin_cap2  = A2; 
  const byte pin_LED = 12;

  const byte ROWS = 4; 
  const byte COLS = 3; 
  char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
  };
  byte rowPins[ROWS] = {12, 11, 10, 9}; 
  byte colPins[COLS] = {8, 7, 6}; 
  Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
  float range,height=1;


void setup() {
  
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" STARTING.. ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ENTER DISTANCE ");
  lcd.setCursor(0,1);
  char Key[10] = customKeypad.getKey();
  int dist[10]=100;
  if (Key!='#' && key!='*'){
    Serial.println(Key); 
  }
  else (Key == '#'){
    break;
  }
  dist = atoi(Key);
  dist = dist/100;
  pinMode(pin_pulse1, OUTPUT);
  digitalWrite(pin_pulse1, LOW);
  pinMode(pin_cap1, INPUT);

  pinMode(pin_pulse2, OUTPUT);
  digitalWrite(pin_pulse2, LOW);
  pinMode(pin_cap2, INPUT);
  
  pinMode(pin_LED, OUTPUT);
  digitalWrite(pin_LED, LOW);
  }
  const int nmeas1 = 256; 
  long int sumsum1 = 0; 
  long int skip1 = 0; 
  long int diff1 = 0;     
  long int flash_period1 = 0; 
  long unsigned int prev_flash1 = 0; 
  const int nmeas2 = 256; 
  long int sumsum2 = 0; 
  long int skip2 = 0; 
  long int diff2 = 0;     
  long int flash_period2 = 0; 
  long unsigned int prev_flash2 = 0;
  
 
void loop() {
  coil1();
  coil2();
  
  if(ledstat1 == 2 && flag1==0){timer1 = millis(); flag1=1;}

  if(ledstat2 == 2 && flag2==0){timer2 = millis(); flag2=1;}

  if (flag1==1 && flag2==1){
     if(timer1 > timer2){Time = timer1 - timer2;}
     else if(timer2 > timer1){Time = timer2 - timer1;}
       Time=Time/1000;
       speed=(distance/Time);
       speed=speed*3600;
       speed=speed/1000;
       range=(speed*sqrt(2*height/9.81));
       }
  if(speed==0){ 
   lcd.setCursor(0, 1); 
  if(flag1==0 && flag2==0){lcd.print("Nothing detected");}
  else{lcd.print("finding... ");} 
  }
  else{
       lcd.clear(); 
       lcd.setCursor(0, 0); 
       lcd.print("Speed:");
       lcd.print(speed,1);
       lcd.print("Km/Hr ");
       lcd.setCursor(0, 1); 
      if(range > dist){lcd.print("Enough Speed"); }
            else{lcd.print(" Low Speed  "); }    
    delay(3000);
    digitalWrite(led, LOW);
    speed = 0;
    flag1 = 0;
    flag2 = 0;    
    }

void coil1(){
    int minval = 2000;
    int maxval = 0;
   
    long unsigned int sum1 = 0;
  for (int imeas1 = 0; imeas1 < nmeas1 + 2; imeas1++) {
    pinMode(pin_cap1, OUTPUT);
    digitalWrite(pin_cap1, LOW);
    delayMicroseconds(20);
    pinMode(pin_cap1, INPUT);
    
  for (int ipulse1 = 0; ipulse1 < npulse1; ipulse1++) {
    digitalWrite(pin_pulse1, HIGH); 
    delayMicroseconds(3);
    digitalWrite(pin_pulse1, LOW);
    delayMicroseconds(3);
    }
       
    int val1 = analogRead(pin_cap1); 
    minval1 = min(val1, minval1);
    maxval1 = max(val1, maxval1);
    sum1 += val1;
    
    long unsigned int timestamp1 = millis();
    byte ledstat1 = 0;
  if (timestamp1 < prev_flash1 +12) {
  if (diff1 > 0)ledstat1 = 1;
  if (diff1 < 0)ledstat1 = 2;
     }
  if (timestamp1 > prev_flash1 + flash_period1) {
  if (diff1 > 0)ledstat1 = 1;
  if (diff1 < 0)ledstat1 = 2;
     prev_flash1 = timestamp1;
     }
  if (flash_period1 > 1000)ledstat1 = 0;

  if (ledstat1 == 0) {
      digitalWrite(pin_LED, LOW);
      }
  if (ledstat1 == 1) {
      digitalWrite(pin_LED, LOW);
      }
  if (ledstat1 == 2) {
      digitalWrite(pin_LED, HIGH);
      }
    }
 
    sum1 -= minval1; sum1 -= maxval1;
    
  if (sumsum1 == 0) sumsum1 = sum1 << 6; 
     long int avgsum1 = (sumsum1 + 32) >> 6;
     diff1 = sum1 - avgsum1;
  if (abs(diff1)<avgsum1 >> 10) {   
    sumsum1 = sumsum1 + sum1 - avgsum1;
    skip1 = 0;
    }
  else {
    skip1++;
    }
  if (skip1 > 64) { 
    sumsum1 = sum1 << 6;
    skip1 = 0;
    }
                                          
  if (diff1 == 0) flash_period1 = 1000000;
  else flash_period1 = avgsum1 / (2 * abs(diff1));
   }

   
void coil2(){
    int minva2 = 2000;
    int maxva2 = 0;
   
    long unsigned int sum2 = 0;
  for (int imeas2 = 0; imeas2 < nmeas2 + 2; imeas2++) {
    pinMode(pin_cap2, OUTPUT);
    digitalWrite(pin_cap2, LOW);
    delayMicroseconds(20);
    pinMode(pin_cap2, INPUT);
    
  for (int ipulse2 = 0; ipulse2 < npulse2; ipulse2++) {
    digitalWrite(pin_pulse2, HIGH); 
    delayMicroseconds(3);
    digitalWrite(pin_pulse2, LOW);
    delayMicroseconds(3);
    }
       
    int val2 = analogRead(pin_cap2); 
    minval2 = min(val2, minval2);
    maxval2 = max(val2, maxval2);
    sum2 += val2;
    
    long unsigned int timestamp2 = millis();
    byte ledstat2 = 0;
  if (timestamp2 < prev_flash2 +12) {
  if (diff2 > 0)ledstat2 = 1;
  if (diff2 < 0)ledstat2 = 2;
     }
  if (timestamp2 > prev_flash2 + flash_period2) {
  if (diff2 > 0)ledstat2 = 1;
  if (diff2 < 0)ledstat2 = 2;
     prev_flash2 = timestamp2;
     }
  if (flash_period2 > 1000)ledstat2 = 0;

  if (ledstat2 == 0) {
      digitalWrite(pin_LED, LOW);
      }
  if (ledstat2 == 1) {
      digitalWrite(pin_LED, LOW);
      }
  if (ledstat2 == 2) {
      digitalWrite(pin_LED, HIGH);
      }
    }
 
    sum2 -= minval2; sum2 -= maxval2;
    
  if (sumsum2 == 0) sumsum2 = sum2 << 6; 
     long int avgsum2 = (sumsum2 + 32) >> 6;
     diff2 = sum2 - avgsum2;
  if (abs(diff2)<avgsum2 >> 10) {   
    sumsum2 = sumsum2 + sum2 - avgsum2;
    skip2 = 0;
    }
  else {
    skip2++;
    }
  if (skip2 > 64) { 
    sumsum2 = sum2 << 6;
    skip2 = 0;
    }
                                          
  if (diff2 == 0) flash_period2 = 1000000;
  else flash_period2 = avgsum2 / (2 * abs(diff2));
   }
  
  
}
