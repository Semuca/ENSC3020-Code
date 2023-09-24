/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/
#include <TFT_eSPI.h>
#include <SPI.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>

const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;



// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(2);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);
#define VRX_PIN  A8 // Arduino pin connected to VRX pin
#define VRY_PIN  A9 // Arduino pin connected to VRY pin

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int xSpeed = 0; // To Speed value of the X axis

void joystick()
{
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
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

void setup() {
  Wire.begin();
  Serial.begin(9600);         // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  // turn on motor
  myMotor->run(RELEASE);
}

void loop() {

   dist();

  Serial.print("Distance: ");
  Serial.println(distance);
  joystick ();
  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(", y = ");
  Serial.println(yValue);
  delay(200);
  
  if(  xValue < 490)
  {
    myMotor->run(BACKWARD);  
    xSpeed =  490 - xValue;
    myMotor->setSpeed(xSpeed/2);
    }
    else if(xValue > 510)
    {
      myMotor->run(FORWARD);
      xSpeed =  xValue - 510;
      myMotor->setSpeed(xSpeed/2);
      }
      else
      {
         myMotor->run(RELEASE);

        }
 
//  while(distance < 4)
//  {
//    myMotor->run(RELEASE);
//    
//    }

}
