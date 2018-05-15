
#include <Arduino.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H


#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef ESP32
   #define STMPE_CS 32
   #define TFT_CS   21
   #define TFT_DC   33
   #define SD_CS    14
#endif
#if defined(__AVR__) || defined(ARDUINO_SAMD_FEATHER_M0)
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef TEENSYDUINO
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#endif
#ifdef ARDUINO_FEATHER52 
   #define STMPE_CS 30
   #define TFT_CS   31
   #define TFT_DC   11
   #define SD_CS    27
#endif


#define BMP_FILENAME "/Phoenix-Rising-art.bmp"
#define WIFI_CONF_FILENAME "/wifi.cfg"
#define WIFI_REQUEST_TOTAL_FILENAME "/wifi_count"
#define BUFFPIXEL 40

const String ERR_SETUP_TFT = "Couldn't start touchscreen controller";

// UNCLEAR whether we'll use rotation; need to fix UI::bmpDraw()...
// 0 is portrait
// 1,2,3 I can't remember... 
#define TFT_ROTATION 3

#define TS_BUTTON_GRID false
#define TS_BUTTON_SIZE 20

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3

const int HTTP_SERVER_PORT = 80;

const int BAT_RATED_HRS = 20;
const int BAT_RATED_CAP = 205;
const int BAT_PEUKERT   = 1.3;

const float ACT_THRESH  = 4.0;
const bool  SLEEP_MODE  = false;
const float SNAPMULTI   = 0.01; // DEFAULT: 0.01 - so smoot
const int   AMMETER_PIN = A3;
const int VOLTMETER_PIN = A4;

const int VCC = 5000;

const int   R1          = 40; // 40k
const int   R2          = 10; // 10k

#endif
