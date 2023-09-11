#include <WiFi.h>
#include <esp_now.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI(135, 240);
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("SETTING UP...");
  tft.init();
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("TTGO", "TTGO");
  tft.fillScreen(TFT_GREEN);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address:");
  Serial.println(myIP);
  server.begin();
}

void loop() {
  WiFiClient client = server.accept();

  if (client) {
    Serial.println("NEW CONECTION FOUND");
    while (client.connected()) {
      Serial.println("MANAGING CONNECTION");
      if (client.available()) {
        tft.fillScreen(TFT_BLUE);
      }
    }
  }
  tft.fillScreen(TFT_GREEN);
}
