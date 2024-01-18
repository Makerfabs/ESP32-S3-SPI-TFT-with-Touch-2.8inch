/*
使用 1.3.1  版本的库 GFX_Library_for_Arduino 在文件夹： C:\Users\maker\Documents\Arduino\libraries\GFX_Library_for_Arduino
使用 2.0.0  版本的库 SPI 在文件夹： C:\Users\maker\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SPI
使用 2.0.0  版本的库 SD_MMC 在文件夹： C:\Users\maker\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SD_MMC
使用 2.0.0  版本的库 FS 在文件夹： C:\Users\maker\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\FS
使用 1.0.1  版本的库 bb_captouch 在文件夹： C:\Users\maker\Documents\Arduino\libraries\bb_captouch
使用 2.0.0  版本的库 Wire 在文件夹： C:\Users\maker\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\Wire
使用 1.2.8  版本的库 JPEGDEC 在文件夹： C:\Users\maker\Documents\Arduino\libraries\JPEGDEC

*/

#include <Arduino_GFX_Library.h>
#include "SD_MMC.h"
#include <FS.h>
#include <bb_captouch.h>
#include "esp_camera.h"
#include "JpegFunc.h"

#define CAMERA_MODEL_MAKERFABS
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

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
#define TOUCH_RST 1

#define PIN_SD_CMD 2
#define PIN_SD_CLK 42
#define PIN_SD_D0 41

#define JPEG_FILENAME_LOGO "/logo_240240.jpg"

Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO, HSPI, true); // Constructor
Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RES, 0 /* rotation */, true /* IPS */);
BBCapTouch bbct;

void setup()
{
    esp32_init();

    Serial.println("Color");
    gfx->fillScreen(RED);
    delay(1000);
    gfx->fillScreen(GREEN);
    delay(1000);
    gfx->fillScreen(BLUE);
    delay(1000);
    gfx->fillScreen(BLACK);
    delay(1000);
    gfx->fillScreen(WHITE);
    Serial.println("Color Over");

    gfx->fillRect(200, 280, 40, 40, BLUE);
    gfx->setTextSize(1);
    gfx->setCursor(210, 290);
    gfx->setTextColor(YELLOW);
    gfx->println(F("CAM"));

    while (1)
    {
        uint16_t x, y;

        if (get_touch(&x, &y))
        {
            if (x > 200 && x < 240 && y > 280 && y < 320)
                break;
            Serial.print(x);
            Serial.print(",");
            Serial.println(y);

            gfx->fillCircle(x, y, 6, RED);
        }
        delay(10);
    }

    gfx->fillScreen(BLACK);
    camera_init_s3();
}

void loop()
{
    long runtime = millis();

    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();

    // char s[20];
    // sprintf(s, "x %d y %d", fb->width, fb->height);
    // Serial.println(s);

    gfx->draw16bitBeRGBBitmap(0, 0, (uint16_t *)fb->buf, fb->width, fb->height);

    esp_camera_fb_return(fb);

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
    gfx->fillScreen(WHITE);
    gfx->setTextSize(4);
    gfx->setTextColor(RED);

    // Touch init

    bbct.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

    // SDMMC init

    SD_MMC.setPins(PIN_SD_CLK, PIN_SD_CMD, PIN_SD_D0);
    if (!SD_MMC.begin("/sdcard", true, true))
    {
        Serial.println("Card Mount Failed");
        gfx->println(F("No SD Card"));
    }
    else
        jpegDraw(JPEG_FILENAME_LOGO, jpegDrawCallback, true /* useBigEndian */,
                 0 /* x */, 0 /* y */, 240 /* widthLimit */, 320 /* heightLimit */);
    delay(2000);

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
        s->set_vflip(s, 1);      // flip it back
        s->set_brightness(s, 1); // up the brightness just a bit
        s->set_saturation(s, 0); // lower the saturation
    }
}

//--------------------------

// pixel drawing callback
static int jpegDrawCallback(JPEGDRAW *pDraw)
{
    // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}

int get_touch(uint16_t *x, uint16_t *y)
{
    TOUCHINFO ti;
    if (bbct.getSamples(&ti))
    {
        // *x = ti.y[0];
        // *y = map(ti.x[0], 240, 0, 0, 240);

        *x = ti.x[0];
        *y = ti.y[0];

        return 1;
    }
    else
        return 0;
}