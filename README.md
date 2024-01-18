# Matouch ESP32-S3 SPI TFT with Touch 2.8" 

```
Version:        V1.0
Author:            Vincent
Create Date:    2024/1/18
Note:

```

[TOC]

![main](md_pic/main.jpg)

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

# Matouch ESP32-S3 SPI TFT with Touch 2.8" 

## Intruduce

Product Link ：[]()

Wiki Link : []()





## Feature

- Controller: ESP32-S3
- Wireless: WiFi& Bluetooth 5.0
- LCD: 2.8", 320*240 resolution, driver ST7789


# Code

## Complier Option

- Install board : ESP32 .
- Dependency libraries：

```
1.3.1  Version GFX_Library_for_Arduino
2.0.0  Version SPI
2.0.0  Version SD_MMC 
2.0.0  Version FS
1.0.1  Version bb_captouch 
2.0.0  Version Wire 
1.2.8  Version JPEGDEC 
```

- Use type-c use cable connect USB to PC.
- Select "ESP32-S3 DEV Module"

"mf_Lvgl" library is modified from Lvgl library (version 8.3.2). 

If you want use lvgl, please unzip "lib/mf_Lvgl.zip" and copy to arduino library directory.

## Factory test program

\example\fw_test_v2

You can run this file test if you feel that your hardware is not working properly. 

Need copy logo_240240.jpg to sd card.

