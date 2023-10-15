#include "esp_camera.h"
#include "fd_forward.h"
#include "fb_gfx.h"
//#include "dl_lib_matrix3d.h"
#define BAUD_RATE 115200
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char* ssid = "Fold";
const char* password = "vrte5372";
void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  //esp_camera_deinit();
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QQVGA);
}


// Function to convert RGB888 to RGB565
void convertRGB565(unsigned char r, unsigned char g, unsigned char b,
                             unsigned char *b1, unsigned char *b2)
{ unsigned short rgb;
  // Extracting the RGB565 components from RGB888
  unsigned short red   = (r >> 3) & 0x1F;
  unsigned short green = (g >> 2) & 0x3F;
  unsigned short blue  = (b >> 3) & 0x1F;
    
  // Combining the components into RGB565 format
  rgb = (red << 11) | (green << 5) | blue;
  *b1 = (rgb >> 8) & 0xFF;
  *b2 =  rgb & 0xFF;
}


// -----------------------------------------------------------

void loop()
{ camera_fb_t * fb;
  char str[50];
  int pos;
  uint8_t b1, b2;
    
  if (Serial.available())
  { 
    // Wait for a character from the serial interface
    char c = Serial.read();

    if (c == 'c')
    {
      // Capture and transmit the image
      fb = esp_camera_fb_get();
      if (!fb)
      {
        Serial.println("Failed to capture image");
        return;
      }
      //As output, this function will return a pointer to the allocated matrix struct.
      dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
      /*
      void *ptrVal = NULL;                                                                                 // create a pointer for memory location to store the data
      uint32_t ARRAY_LENGTH = fb->width * fb->height * 3;
      ptrVal = heap_caps_malloc(ARRAY_LENGTH, MALLOC_CAP_SPIRAM); 
      uint8_t *image_matrix = (uint8_t *)ptrVal;               
      */
      //to convert the captured image to RGB565 format . Sign "->" allows to access elements in Structures
      fmt2rgb888(fb->buf, fb->len, fb->format, image_matrix->item);
 
      // Transmit the image over serial
      Serial.print("P0");
      //Serial.write((const uint8_t *)fb->buf, fb->len);
      //Serial.write((const uint8_t *)image_matrix->item, fb->len);
      pos = 0;
      for (int i=0; i<fb->height; i++)
      { for (int j=0; j<fb->width; j++)
        { uint8_t a = (image_matrix->item)[pos++] ;
          uint8_t b = (image_matrix->item)[pos++] ;
          uint8_t c = (image_matrix->item)[pos++] ;
          convertRGB565(a,b,c, &b1, &b2);
          Serial.write(b1); Serial.write(b2);
          //sprintf(str, "%3d %3d %3d ", a,b,c);
          //Serial.print(str);
        }
        //Serial.println("");
      }

      // Release the frame bufferp
      esp_camera_fb_return(fb);
      dl_matrix3du_free(image_matrix);
    }
  }
}
