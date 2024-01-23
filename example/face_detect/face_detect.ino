
/*

使用 1.3.1  版本的库 GFX_Library_for_Arduino 在文件夹： C:\Users\maker\Documents\Arduino\libraries\GFX_Library_for_Arduino 
使用 2.0.0  版本的库 SPI 在文件夹： C:\Users\maker\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SPI 

*/


#include <Arduino_GFX_Library.h>
#include "esp_camera.h"

#define CAMERA_MODEL_MAKERFABS
#include "camera_pins.h"
#include "face_dect.hpp"

#define TFT_BLK 48
#define TFT_RES -1

#define TFT_CS 15
#define TFT_MOSI 13
#define TFT_MISO 12
#define TFT_SCLK 14
#define TFT_DC 21

#define TOUCH_INT 40
#define TOUCH_SDA 39
#define TOUCH_SCL 38
#define TOUCH_RST 16

Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO, HSPI, true); // Constructor
Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RES, 0 /* rotation */, true /* IPS */);

void setup()
{
    esp32_init();
    camera_init_s3();
}

void loop()
{
    long runtime = millis();
    face_dect_display();

    gfx->setCursor(10, 280);
    gfx->fillRect(10, 280, 50, 8, BLUE);

    runtime = millis() - runtime;
    char s[20];
    sprintf(s, "FPS:%.1f", 1000.0 / runtime);

    gfx->println(s);
}

// ILI9488 init and SD card init
void esp32_init()
{
    Serial.begin(115200);
    Serial.println("S3 2.8 SPI Test!");

    // TFT init

    pinMode(TFT_BLK, OUTPUT);
    digitalWrite(TFT_BLK, 1);

    delay(1000);
    Serial.println("start");

    gfx->begin();
    gfx->fillScreen(BLACK);
    Serial.println("TFT init over.");
}

// Camera setting
void camera_init_s3()
{
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
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_240X240;
    // config.pixel_format = PIXFORMAT_JPEG; // for streaming
    config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 2;

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t *s = esp_camera_sensor_get();
    if (s->id.PID == OV3660_PID)
    {
        s->set_vflip(s, 1);       // flip it back
        s->set_brightness(s, 1);  // up the brightness just a bit
        s->set_saturation(s, -2); // lower the saturation
    }
}

void face_dect_display()
{

    // 获取一帧
    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();

    if (!fb)
    {
        Serial.println("Camera capture failed");
        return;
    }

    size_t out_len, out_width, out_height;
    uint8_t *out_buf;
    bool s;

    int face_id = 0;

    HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
    HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
    std::list<dl::detect::result_t> &candidates = s1.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3});
    std::list<dl::detect::result_t> &results = s2.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3}, candidates);

    if (results.size() > 0)
    {
        Serial.println("111111");
        fb_data_t rfb;
        rfb.width = fb->width;
        rfb.height = fb->height;
        rfb.data = fb->buf;
        rfb.bytes_per_pixel = 2;
        rfb.format = FB_RGB565;

        draw_face_boxes(&rfb, &results, face_id);
    }
    gfx->draw16bitBeRGBBitmap(0, 0, (uint16_t *)fb->buf, fb->width, fb->height);

    esp_camera_fb_return(fb);
}
