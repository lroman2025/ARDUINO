#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

int receiver = 8; // Signal Pin of IR receiver to Arduino Digital Pin 11
int mSpeed = 50;
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(4);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(3);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  irrecv.enableIRIn();

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor1->setSpeed(mSpeed);
  myMotor2->setSpeed(mSpeed);
  // turn on motor
  myMotor1->run(RELEASE);
  myMotor2->run(RELEASE);
}

void loop() {
//  uint8_t i;

  if (irrecv.decode(&results)){
    translateIR();
    irrecv.resume();
  }

//  myMotor1->run(BACKWARD);
//  myMotor2->run(FORWARD);
}

void driveForvard(){
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
}

void stop(){
  myMotor1->run(RELEASE);
  myMotor2->run(RELEASE);
}

void driveBackvard(){
  myMotor1->run(BACKWARD);
  myMotor2->run(BACKWARD);
}

void turnLeft(){
  myMotor1->run(FORWARD);
  myMotor2->run(BACKWARD);  
}

void turnRight(){
  myMotor1->run(BACKWARD);
  myMotor2->run(FORWARD);
}

void setSpeed(int val){
  myMotor1->setSpeed(mSpeed*val);
  myMotor2->setSpeed(mSpeed*val);
}

void translateIR(){

  switch (results.value){
    case 16753245: Serial.println(" 1"); setSpeed(1); break;
    case 0xFF629D: Serial.println(" 2"); setSpeed(2); break;
    case 16769565: Serial.println(" 3"); setSpeed(3); break;
    case 0xFF22DD: Serial.println(" 4");    break;
    case 0xFF02FD: Serial.println(" 5");    break;
    case 0xFFC23D: Serial.println(" 6");   break;
    case 16769055: Serial.println(" 7");   break;
    case 0xFFA857: Serial.println(" 8"); break;
    case 16748655: Serial.println(" 9"); break;
    case 0xFF6897: Serial.println("STOP"); stop();    break;
    case 0xFF9867: Serial.println(" 0");    break;
    case 0xFFB04F: Serial.println(" #");    break;
    case 0xFF30CF: Serial.println(" xx");    break;
    case 0xFF18E7:
      Serial.println("FORWARD");
      driveForvard();
      break;
    case 0xFF7A85: Serial.println(" xxx");    break;
    case 0xFF10EF: turnLeft(); Serial.println("LEFT");    break;
    case 0xFF38C7: Serial.println(" OK");    break;
    case 0xFF5AA5: turnRight(); Serial.println("RIGHT");    break;
    case 0xFF42BD: Serial.println(" x");    break;
    case 0xFF4AB5: driveBackvard(); Serial.println("BACKWARD");    break;
    case 0xFF52AD: Serial.println(" #");    break;

    default:
      Serial.println(" other button   ");
      Serial.println(results.value);
  }
}
