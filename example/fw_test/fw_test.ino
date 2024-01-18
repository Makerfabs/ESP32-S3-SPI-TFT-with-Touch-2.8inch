#include <Arduino_GFX_Library.h>
#include "SD_MMC.h"
#include <FS.h>
#include <bb_captouch.h>
#include "JpegFunc.h"

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
    Serial.begin(115200);

    pinMode(TFT_BLK, OUTPUT);
    digitalWrite(TFT_BLK, 1);

    delay(1000);
    Serial.println("start");

    gfx->begin();
    gfx->fillScreen(WHITE);
    gfx->setTextSize(4);
    gfx->setCursor(10, 10);
    gfx->setTextColor(RED);

    bbct.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

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

    
    gfx->println(F("Touch"));

    Serial.println("Text over");
}

void loop(void)
{
    uint16_t x, y;

    if (get_touch(&x, &y))
    {

        Serial.print(x);
        Serial.print(",");
        Serial.println(y);

        gfx->fillCircle(x, y, 6, RED);
    }
    delay(10);
}

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