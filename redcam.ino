#include "esp_camera.h"

#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

static camera_config_t camera_config = {
  .pin_pwdn = CAM_PIN_PWDN,
  .pin_reset = CAM_PIN_RESET,
  .pin_xclk = CAM_PIN_XCLK,
  .pin_sccb_sda = CAM_PIN_SIOD,
  .pin_sccb_scl = CAM_PIN_SIOC,

  .pin_d7 = CAM_PIN_D7,
  .pin_d6 = CAM_PIN_D6,
  .pin_d5 = CAM_PIN_D5,
  .pin_d4 = CAM_PIN_D4,
  .pin_d3 = CAM_PIN_D3,
  .pin_d2 = CAM_PIN_D2,
  .pin_d1 = CAM_PIN_D1,
  .pin_d0 = CAM_PIN_D0,
  .pin_vsync = CAM_PIN_VSYNC,
  .pin_href = CAM_PIN_HREF,
  .pin_pclk = CAM_PIN_PCLK,

  .xclk_freq_hz = 20000000,
  .ledc_timer = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,
  
  .pixel_format = PIXFORMAT_RGB565,
  .frame_size = FRAMESIZE_QVGA,

  .jpeg_quality = 60,
  .fb_count = 1,
  .fb_location = CAMERA_FB_IN_PSRAM,
  .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

char* columns;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  esp_err_t err = esp_camera_init(&camera_config);
  esp_camera_return_all();

  columns = (char*)calloc(320, sizeof(uint8_t));

  pinMode(14, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  camera_fb_t *pic = esp_camera_fb_get();

  for (int i = 0; i < pic->width; i++) {
    columns[i] = 0;
  }

  for (int i = 0; i < pic->height; i++) {
    for (int j = 0; j < pic->width / 2; j++) {
      int pos = i * pic->width + 2 * j;
      int redVal = (pic->buf[pos] & 248) >> 3; // 1111 1000 = 128 + 64 + 32 + 16 + 8 = 248
      int greenVal = ((pic->buf[pos] & 7) << 3) + ((pic->buf[pos + 1] & 224) >> 5);
      int blueVal = (pic->buf[pos + 1] & 31);
      
      if(redVal > 10 && greenVal < 30 && blueVal < 20) {
        columns[j]++;
      }
    }
  }

  char m = 0;

  for (int i = 0; i < pic->width; i++) {
    if (columns[i] > columns[m]) {
      m = i;
    }
  }
  analogWrite(14, (int)m);

  esp_camera_fb_return(pic);
  
  delay(500);
}
