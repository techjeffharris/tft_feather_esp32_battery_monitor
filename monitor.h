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
#include <DNSServer.h>

#include "constants.h"
#include "ammeter.h"
#include "voltmeter.h"

class PbMonitor
{

  public:
    PbMonitor() {};
    bool begin();
    void captivePortal();
    void checkForTouch();
    void checkForWiFiClient();

  protected:
    Ammeter             _ammeter;
    Voltmeter           _voltmeter;
    Adafruit_HX8357     _tft = Adafruit_HX8357(TFT_CS, TFT_DC);
    Adafruit_STMPE610   _ts = Adafruit_STMPE610(STMPE_CS);
    IPAddress           _apIP = IPAddress(WIFI_AP_IP_0, WIFI_AP_IP_1, WIFI_AP_IP_2, WIFI_AP_IP_3);
    IPAddress           _apNetmask = IPAddress(WIFI_AP_NETMASK_0, WIFI_AP_NETMASK_1, WIFI_AP_NETMASK_2, WIFI_AP_NETMASK_3);
    DNSServer           _dnsServer;
    WiFiServer          _httpServer = WiFiServer(HTTP_SERVER_PORT);

    int _currentcolor;
    int _oldcolor;
    int _lastBox_x;
    int _lastBox_y;

    void hideGrid();
    void showGrid();


    bool setBackgroundImage(char *filename, uint8_t x, uint16_t y);
    
    bool setupSD();
    void setupTFT();
    void setupTS();
    bool setupWifi();

    void fingerPaint(int x, int y);

    uint16_t read16(File &f);
    uint32_t read32(File &f);
    
    void cardInfo();
    void sdfstest();

    void tstest ();

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
