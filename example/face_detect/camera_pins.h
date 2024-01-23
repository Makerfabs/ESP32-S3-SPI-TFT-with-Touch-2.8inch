
#if defined(CAMERA_MODEL_MAKERFABS)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1

// ESP32
// #define XCLK_GPIO_NUM 32 // CSI_MCLK
// #define SIOD_GPIO_NUM 26 // TWI_SDA
// #define SIOC_GPIO_NUM 27 // TWI_SCK

// #define Y9_GPIO_NUM 35    // CSI D7
// #define Y8_GPIO_NUM 34    // CSI D6
// #define Y7_GPIO_NUM 39    // CSI D5
// #define Y6_GPIO_NUM 36    // CSI D4
// #define Y5_GPIO_NUM 21    // CSI D3
// #define Y4_GPIO_NUM 19    // CSI D2
// #define Y3_GPIO_NUM 18    // CSI D1
// #define Y2_GPIO_NUM 5     // CSI D0
// #define VSYNC_GPIO_NUM 25 // CSI VSYNC
// #define HREF_GPIO_NUM 23  // CSI HSYNC
// #define PCLK_GPIO_NUM 22  // CSI PCLK

// ESP32S3 3.5 SPI 9488 
// #define XCLK_GPIO_NUM 9  // CSI_MCLK
// #define SIOD_GPIO_NUM 38 // TWI_SDA
// #define SIOC_GPIO_NUM 39 // TWI_SCK

// #define Y9_GPIO_NUM 46    // CSI D7
// #define Y8_GPIO_NUM 3     // CSI D6
// #define Y7_GPIO_NUM 8     // CSI D5
// #define Y6_GPIO_NUM 16    // CSI D4
// #define Y5_GPIO_NUM 6     // CSI D3
// #define Y4_GPIO_NUM 4     // CSI D2
// #define Y3_GPIO_NUM 5     // CSI D1
// #define Y2_GPIO_NUM 7     // CSI D0
// #define VSYNC_GPIO_NUM 11 // CSI VSYNC
// #define HREF_GPIO_NUM 10  // CSI HSYNC
// #define PCLK_GPIO_NUM 17  // CSI PCLK

// ESP32S3 2.8 SPI ST7789 
#define XCLK_GPIO_NUM 9  // CSI_MCLK
#define SIOD_GPIO_NUM 39 // TWI_SDA
#define SIOC_GPIO_NUM 38 // TWI_SCK

#define Y9_GPIO_NUM 46    // CSI D7
#define Y8_GPIO_NUM 3     // CSI D6
#define Y7_GPIO_NUM 8     // CSI D5
#define Y6_GPIO_NUM 16    // CSI D4
#define Y5_GPIO_NUM 6     // CSI D3
#define Y4_GPIO_NUM 4     // CSI D2
#define Y3_GPIO_NUM 5     // CSI D1
#define Y2_GPIO_NUM 7     // CSI D0
#define VSYNC_GPIO_NUM 11 // CSI VSYNC
#define HREF_GPIO_NUM 10  // CSI HSYNC
#define PCLK_GPIO_NUM 17  // CSI PCLK

#else
#error "Camera model not selected"
#endif
