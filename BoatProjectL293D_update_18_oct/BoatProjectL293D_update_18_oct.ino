//#include <analogWrite.h>
#include <Wire.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_now.h>

//const char* ssid = "Fold";
//const char* password = "vrte5372";
//
//const char* camServer = "http://192.168.4.1/cam";
//
//unsigned long previousMillis = 0;
//const long interval = 200; 
//
//String camVal;

typedef struct struct_message {
    int a;
} struct_message;

// Create a struct_message called myData
struct_message myData;

//motor A connections
int enA = 12;
int in1 = 27; 
int in2 = 33;
// Motor B connections
int enB = 13;
int in3 = 25;
int in4 = 26;
int cam = 2;
int velocity = 225;
int velocity_off = 0;
const int trigPin = 17;
const int echoPin = 15;
// defines variables
long duration;
int distance;

int x; 
int y;


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Cam Data: ");
  Serial.println(myData.a);
  x = myData.a;
  Serial.print("_____________________________________________________________");
}

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(cam, INPUT);

//  WiFi.begin(ssid, password);
//  Serial.print("Connecting");
//  while(WiFi.status() != WL_CONNECTED) { 
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.print("");
//  Serial.print("Connected to WiFi network with IP Address: ");
//  Serial.print(WiFi.localIP());
//  
  // Turn off motors - Initial state
  Wire.begin();
  Serial.begin(115200);         // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  //AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz


  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

//String httpGETRequest(const char* serverName) {
//  WiFiClient client;
//  HTTPClient http;
//    
//  http.begin(client, serverName);
//  
//  int httpResponseCode = http.GET();
//  
//  String payload = "--"; 
//  
//  if (httpResponseCode>0) {
//    Serial.print("HTTP Response code: ");
//    Serial.print(httpResponseCode);
//    payload = http.getString();
//  }
//  else {
//    Serial.print("Error code: ");
//    Serial.print(httpResponseCode);
//  }
//  http.end();
//
//  return payload;
//}

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
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
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
//  unsigned long currentMillis = millis();
//  if(currentMillis - previousMillis >= interval) {
//  // Check WiFi connection status
//  if(WiFi.status()== WL_CONNECTED ){
//    camVal = httpGETRequest(camServer);
//    x = camVal.toInt();
//    previousMillis = currentMillis;
//  }
//  else {
//    Serial.print("WiFi Disconnected");
//  }
           
  Serial.print("The X-Value is:    ");
  Serial.println(x);
//  Serial.print("The distance is:    ");
//  Serial.println(distance);
  if(distance < 10 ){
    
    stop_motor2();
    stop_motor1();
    }
    else{

//      int i = 0;

//      while (i < 256 ){
//        analogWrite(enB, i);   // see what is the lowest value we can set the motor to to keep it turning
//        digitalWrite(in3, HIGH);
//        digitalWrite(in4, LOW);
//        analogWrite(enA, i);
//        digitalWrite(in1, HIGH);
//        digitalWrite(in2, LOW);
//        Serial.print(" speed is: ");
//        Serial.println(i);
//        delay(250);
//        i++;
//        }
      if( x < 90)
        {
//        velocity = ((x - 110)/110)*75 + 180;
          
        
        analogWrite(enA,velocity_off);
        analogWrite(enB, velocity);
        Serial.println("----left----");
//        Serial.println(velocity);
        motor2forward();
//        motor1forward();
        
        }
        else if(x > 155)
        {
//          velocity =  ((x - 145)/110)*75 + 180;
          
          analogWrite(enA, velocity);
          analogWrite(enB, velocity_off);
          motor1backward();
          Serial.println("right----");
          Serial.println(velocity);
//          motor2backward();
          }
          else
          {
          velocity = 225;
          analogWrite(enA, velocity);
          analogWrite(enB, velocity);
          motor1backward();
          motor2forward();
          Serial.println("straight----");
          Serial.println(velocity);
    
            }
    }
}
  



// This function lets you control speed of the motors
