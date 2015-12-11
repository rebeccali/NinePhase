/*
Project: 6.131 Final Project, Fall, 2015, Prof. Steve Leeb
Author: Rebecca Li
Date Created: 11/22/2015
With help from: Bayley Wang
MIT License

Connect phase A to the green wire, and phase B to the thing. Connect throttle to the servo pot. 

*/
//PINS BASED ON IC3
#define THRTL 2 //Throttle (analog pin)
#define IN_HA 3
#define IN_LA 9
#define IN_HB 10
#define IN_LB 5
#define IN_HC 11
#define IN_LC 6
#define HALL_A 13
#define HALL_B 12
#define HALL_C 8
#define SO1 A0 //Input
#define SO2 A1 //input
#define FAULT A3 //Input, in future use as internal pullup
#define MO_C 4 //Overcurrent enable
#define DC_CAL 7
#define S_POT A6
//Commutation Lookup Table
//{halla hallb hallc, ha la hb lb hc lc}
/*const int commutate[2][6] = {
  {100,010010},
  {010,100100},
  {110,000110},
  {001,001001},
  {101,011000},
  {011,100001},
  }; 
*/
const int pos = 600;
int ha = 0;
int la = 0;
int hb = 0;
int lb = 0;
int hc = 0;
int lc = 0;
int hall[6][3] = {{1,0,0}, {0,1,0}, {1,1,0}, {0,0,1}, {1,0,1}, {0,1,1}};
int phase[6][6] = {{0,1,0,0,1,0}, {1,0,0,1,0,0}, {0,0,0,1,1,0}, {0,0,1,0,0,1}, {0,1,1,0,0,0}, {1,0,0,0,0,1}};
int high[7] = {0,1,2,2,4,1,4};//Ch, Cl, Bh, Bl, Ah, Al 
int low[7] = {0,4,1,4,2,2,1};
//int highccw[7] = {4,1,4,2,2,1};
  //int lowccw[7]={1,2,2,4,1,4}; // these tables are broken
void setup() {
  //Pin Setup
  pinMode(THRTL, INPUT);
  pinMode(MO_C, OUTPUT);
  pinMode(DC_CAL, OUTPUT);
  pinMode(SO1, INPUT);
  pinMode(SO2, INPUT);
  pinMode(FAULT, INPUT);
  pinMode(HALL_A, INPUT);
  pinMode(HALL_B, INPUT);
  pinMode(HALL_C, INPUT);
  pinMode(IN_HA, OUTPUT);
  pinMode(IN_LA, OUTPUT);
  pinMode(IN_HB, OUTPUT);
  pinMode(IN_LB, OUTPUT);
  pinMode(IN_HC, OUTPUT);
  pinMode(IN_LC, OUTPUT);
  pinMode(S_POT, INPUT);

  //Set frequencies for PWM pins
  // Pins 3, 9, 10, and 11 have a base frequency of 31250 Hz
  // Pins 5 and 6 have a base frequen-cy of 62500 Hz
  setPwmFrequency(3,8); //31250/8 = 3906.25 Hz
  setPwmFrequency(9,8); //31250/8 = 3906.25 Hz
  setPwmFrequency(10,8); //31250/8 = 3906.25 Hz
  setPwmFrequency(11,8); //31250/8 = 3906.25 Hz
  setPwmFrequency(5,16); //62500/16 = 3906.25 Hz
  setPwmFrequency(6,16); //62500/16 = 3906.25 Hz
  Serial.begin(9600);
   

  //Initial pin conditions
  digitalWrite(MO_C, HIGH); //HIGH is for overcurrent shut down, LOW is for current control.
  digitalWrite(DC_CAL, LOW); //Turn off DC offset calibration for shunts

}

void loop() {
  
  int thrtl = analogRead(THRTL);//ha positive makes thrtl go up when yellow is on A
  //Serial.println(thrtl);
 // thrtl = map(thrtl, 125, 255, 50, 0);
  //thrtl = constrain(thrtl,0, 50);
  //signed int s_pot = analogRead(6);
  int err = thrtl - pos;
  
  Serial.println(err);
  //err = 0; 
 
int kp = 3;
  
if(err<-10){
  analogWrite(IN_HA, constrain(-err*kp, 0, 200));//constrain(abs(err*2), 0, 200)); //0 to 255 duty cycle
  analogWrite(IN_HB, 0); //0 to 255 duty cycle
  analogWrite(IN_HC, 0); //0 to 255 duty cycle
  digitalWrite(IN_LA, 0);
  digitalWrite(IN_LB, 1);
  digitalWrite(IN_LC, 0);
}
else if(err>10){
  analogWrite(IN_HA, 0); //0 to 255 duty cycle
  analogWrite(IN_HB, constrain(err*kp, 0, 200));//constrain(abs(err*2), 0, 200)); //0 to 255 duty cycle
  analogWrite(IN_HC, 0); //0 to 255 duty cycle
  digitalWrite(IN_LA, 1);
  digitalWrite(IN_LB, 0);
  digitalWrite(IN_LC, 0);
}
else{
 analogWrite(IN_HA, 0); //0 to 255 duty cycle
  analogWrite(IN_HB, 0); //0 to 255 duty cycle
  analogWrite(IN_HC, 0); //0 to 255 duty cycle
  digitalWrite(IN_LA, 0);
  digitalWrite(IN_LB, 0);
  digitalWrite(IN_LC, 0);
}

//Ccw
/*
 analogWrite(IN_HA, 0); //0 to 255 duty cycle
  analogWrite(IN_HB, 100); //0 to 255 duty cycle
  analogWrite(IN_HC, 0); //0 to 255 duty cycle
  digitalWrite(IN_LA, 1);
  digitalWrite(IN_LB, 0);
  digitalWrite(IN_LC, 0);
  delayMicroseconds(4);
 
 //CW 
 analogWrite(IN_HA, 100); //0 to 255 duty cycle
  analogWrite(IN_HB, 0); //0 to 255 duty cycle
  analogWrite(IN_HC, 0); //0 to 255 duty cycle
  digitalWrite(IN_LA, 0);
  digitalWrite(IN_LB, 1);
  digitalWrite(IN_LC, 0);
  delayMicroseconds(4);
*/
}




void test(bool testp) {
    digitalWrite(IN_HA, 0);
    digitalWrite(IN_HB, 0);
    digitalWrite(IN_HC, 0);
    digitalWrite(IN_LA, 0);
    digitalWrite(IN_LB, 0);
    digitalWrite(IN_LC, 0);
    delay(1);
    digitalWrite(IN_HA, 0);
    digitalWrite(IN_HB, 0);
    digitalWrite(IN_HC, 0);
    digitalWrite(IN_LA, 0);
    digitalWrite(IN_LB, 0);
    digitalWrite(IN_LC, 1);
    delay(1);

    digitalWrite(IN_HA, 0);
    digitalWrite(IN_HB, 0);
    digitalWrite(IN_HC, 1);
    digitalWrite(IN_LA, 0);
    digitalWrite(IN_LB, 0);
    digitalWrite(IN_LC, 0);
    delay(1);
    digitalWrite(IN_HA, 0);
    digitalWrite(IN_HB, 0);
    digitalWrite(IN_HC, 0);
    digitalWrite(IN_LA, 0);
    digitalWrite(IN_LB, 0);
    digitalWrite(IN_LC, 0);
    delay(1);
}
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
