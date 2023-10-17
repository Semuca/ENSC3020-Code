#include <analogWrite.h>
#include <Wire.h>

//motor A connections
int enA = 12;
int in1 = 27; 
int in2 = 33;
// Motor B connections
int enB = 13;
int in3 = 25;
int in4 = 26;
int velocity;
int joystick_up_down = 32;
int joystick_left_right = 2;
const int trigPin = 17;
const int echoPin = 15;
// defines variables
long duration;
int distance;

int x; 
int y;
void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(joystick_up_down, INPUT);
  pinMode(joystick_left_right, INPUT);
  // Turn off motors - Initial state
  Wire.begin();
  Serial.begin(115200);         // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  //AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void dist()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  }
// This function lets you control spinning direction of motors
void motor1forward() {
  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void motor1backward() {
  // Turn on motor A & B
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}
void motor2forward() {
  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void motor2backward() {
  // Turn on motor A & B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stop_motor1() {
  // Turn on motor A & B
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  
}
void stop_motor2() {
  // Turn on motor A & B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
}
void loop() {
//  analogWrite(enB, 255);
//  digitalWrite(in3, HIGH);
//  digitalWrite(in4, LOW);
//  analogWrite(enA, 255);
//  digitalWrite(in1, HIGH);
//  digitalWrite(in2, LOW);
   dist();
   Serial.print("The distance is:    ");
   Serial.println(distance);
  if(distance < 10 ){
    
    motor1backward();
    motor2backward();
    delay(2000);
    stop_motor2();
    stop_motor1();
    }
    else{
        analogWrite(enB, 255);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        analogWrite(enA, 255);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
//    if(  x < 120)
//  {
//    velocity = ((135 - x)/135)*255
//      if (velocity < 60) {velocity = 60;}
//    
//    analogWrite(enA, velocity);
//    analogWrite(enB, velocity);
//    motor2forward();
//    motor1forward();
//    
//    }
//    else if(x > 135)
//    {
//      velocity =  abs((140-x)/140)*255;
//      analogWrite(enA, velocity);
//      analogWrite(enB, velocity);
//      motor1backward();
//      motor2backward();
//      }
//      else
//      {
//      motor1backward();
//      motor2forward();
//
//        }
    }

}
  



// This function lets you control speed of the motors
