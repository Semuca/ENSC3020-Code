// 20_Color_Blob_Detector.ino

/**
 * Detect blobs of given color
 */
#define MAX_RESOLUTION_VGA
#define MAX_RECURSION_DEPTH 13

#include "esp32cam.h"
#include "analogWrite.h"
#include "esp32cam/JpegDecoder.h"
#include "esp32cam/apps/ColorBlobDetector.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <esp_now.h>

uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xFC, 0x8A, 0x4C};

typedef struct struct_message {
  int a;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

using namespace Eloquent::Esp32cam;

Cam cam;
JpegDecoder decoder;
Applications::ColorBlobDetector detector(200, 75, 80);

int analogScale;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
  
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);
    
    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }

    Serial.println("Init");

    // Connect to Wi_Fi network with SSID and password
    Serial.print("Setting AP (Access Point)...");
    // Remove the password parameter, if you want the AP (Access Point) to be open
//    WiFi.mode(WIFI_AP);
//    WiFi.softAP(ssid, password);
//    
//    IPAddress IP = WiFi.softAPIP();
//    Serial.print("AP IP address: ");
//    Serial.println(IP);
//
//    // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
//    server.on("/cam", HTTP_GET, [](AsyncWebServerRequest *request){
//      char buf[10];
//      sprintf(buf, "%i", analogScale);
//      request->send_P(200, "text/plain", buf);
//    });
//  
//    server.begin();
//    initWebSocket();
  
    //ws.textAll(analogScale);

    pinMode(14, OUTPUT);

    cam.aithinker();
    cam.highQuality();
    cam.vga();
    cam.highestSaturation();
    cam.disableAutomaticWhiteBalance();
    cam.disableAutomaticExposureControl();
    cam.disableGainControl();

    /**
     * Set detector tollerance
     * The higher, the more shade of colors it will pick
     */
    detector.tollerate(40);
    /**
     * Skip blob localization (slow) if not enough
     * pixels match color
     */
    detector.setMinArea(30 * 30);

    while (!cam.begin())
        Serial.println(cam.getErrorMessage());
}


void loop() {
    if (!cam.capture()) {
        Serial.println(cam.getErrorMessage());
        return;
    }

    if (!decoder.decode(cam)) {
        Serial.println(decoder.getErrorMessage());
        return;
    }

    /**
     * Detect blob in frame
     */
    if (detector.detect(decoder)) {
        Serial.print("Blob detected from top-left ");
        Serial.print(detector.blob.top);
        Serial.print(", ");
        Serial.print(detector.blob.left);
        Serial.print(" to bottom-right ");
        Serial.print(detector.blob.bottom);
        Serial.print(", ");
        Serial.println(detector.blob.right);
        Serial.print("Blob detection run in ");
        Serial.print(detector.getExecutionTimeInMillis());
        Serial.println("ms");

        static int maxRight = 0;
        if (maxRight < detector.blob.right) {
          maxRight = detector.blob.right;
        }
        Serial.print("Max Right: ");
        Serial.println(maxRight);
        
        int xLocation = (detector.blob.left + detector.blob.right) / 2;
        Serial.print("x location: ");
        Serial.println(xLocation);
        analogScale = ((float) xLocation / 80) * 255;
        Serial.print("Analog Scale: ");
        Serial.println(analogScale);

        myData.a = analogScale;

        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
           
        if (result == ESP_OK) {
          Serial.println("Sent with success");
        }
        else {
          Serial.println("Error sending the data");
        }
        
        //analogWrite(14, (int) analogScale);
    }
    else {
        Serial.println(detector.getErrorMessage());
    }
    
    // while debugging, these may turn out useful
    Serial.print(detector.maskCount);
    Serial.println(" pixels match target color");
    //Serial.println(detector.toString());
    detector.printTo(Serial);
    
}
