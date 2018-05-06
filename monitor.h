/*
  Monitor.h - Library header for Power Consumption Monitor
  Created by Jeff A. Harris May 4, 2018
  Released into the public domain.
*/
#ifndef MONITOR_H
#define MONITOR_H

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_HX8357.h>
#include <Adafruit_STMPE610.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>

#include "constants.h"
#include "ammeter.h"
#include "voltmeter.h"

class PbMonitor
{

  public:
    PbMonitor() {};
    bool begin();
    void checkForTouch();
  protected:
    Ammeter           _ammeter;
    Voltmeter         _voltmeter;
    Adafruit_HX8357   _tft = Adafruit_HX8357(TFT_CS, TFT_DC);
    Adafruit_STMPE610 _ts = Adafruit_STMPE610(STMPE_CS);
    WiFiServer        _server = WiFiServer(HTTP_SERVER_PORT);

    int currentcolor, oldcolor, lastTouch_x, lastTouch_y, height, width;

    void hideGrid();
    void showGrid();

    void paint(int x, int y);

    bool setupDisplay();
    bool setBackgroundImage(char *filename, uint8_t x, uint16_t y);
    bool setupWifi();

    uint16_t read16(File &f);
    uint32_t read32(File &f);
    void cardInfo();
    void sdfstest();

    bool tstest ();

    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void createDir(fs::FS &fs, const char * path);
    void removeDir(fs::FS &fs, const char * path);
    String readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void appendFile(fs::FS &fs, const char * path, const char * message);
    void renameFile(fs::FS &fs, const char * path1, const char * path2);
    void deleteFile(fs::FS &fs, const char * path);
    void testFileIO(fs::FS &fs, const char * path);

};

#endif
