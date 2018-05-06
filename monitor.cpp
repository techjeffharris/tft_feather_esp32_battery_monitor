
#include "monitor.h"
#include <WiFi.h>

bool PbMonitor::begin() {

  // initialize the SD card
  Serial.print("Initializing SD card...");
  if (! SD.begin(SD_CS)) {
  Serial.println("failed!");
  return false;
  } else {
  // display SD Card information
  cardInfo();

  setupDisplay();  

  // Initilize WiFi
  setupWifi();

  // setup the display and load background image

  Serial.println("PbMonitor has begun.");
  return true;
  }


  
}



bool PbMonitor::setupDisplay() {
  Serial.println("HX8357D Featherwing Initializing..."); 
  
  if (!_ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }

  Serial.println("Touchscreen started");

  // Uncomment these lines to turn off the backlite via STMPE!
  //_ts.writeRegister8(STMPE_GPIO_DIR, 1<<2);
  //_ts.writeRegister8(STMPE_GPIO_ALT_FUNCT, 1<<2);
  //_ts.writeRegister8(STMPE_GPIO_CLR_PIN, 1<<2);
  
  _tft.begin();

  width = _tft.width();
  height = _tft.height();

  Serial.printf("width: %i\nheight: %i\n", width, height);

  _tft.setRotation(TFT_ROTATION);

  width = _tft.width();
  height = _tft.height();

  Serial.printf("width: %i\nheight: %i\n", width, height);

  _tft.fillScreen(HX8357_WHITE);
  setBackgroundImage(BMP_FILENAME, 0, 0);

  // _tft.setCursor(240,170);
  // _tft.setTextColor(HX8357_BLUE, HX8357_WHITE);
  // _tft.setTextSize(3);
  // _tft.print("foo");

  if (TS_BUTTON_GRID) {
    showGrid();
  }
 
  // select the current color 'red'
  // currentcolor = HX8357_RED;

  // turn backlite on via STMPE
  //_ts.writeRegister8(STMPE_GPIO_SET_PIN, 1<<2);
  //
  
  return true;
}

bool PbMonitor::setupWifi() {

  String ssid, psk, wifi_info;

  // read file /wifi.cfg:
  // | ssid
  // | psk
  wifi_info = readFile(SD, "/wifi.cfg");

  Serial.println("wifi_info: " + wifi_info);

  int nlpos = wifi_info.indexOf("\n");

  Serial.println("nlpos: " + String(nlpos));

  ssid = wifi_info.substring(0, nlpos);
  psk = wifi_info.substring(nlpos+1);

  char ssid_buf[32];
  char psk_buf[64];

  ssid.toCharArray(ssid_buf, 32);
  psk.toCharArray(psk_buf, 64);

  // parse file for ssid and psk
  const char* ssid_ptr = ssid_buf;
  const char* psk_ptr = psk_buf;

  Serial.println("ssid: " + ssid + "\t psk: " + psk);

  // start wifi
  WiFi.begin(ssid_ptr, psk_ptr);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // _server.begin();

  return true;

};



void PbMonitor::checkForTouch() {
 
  // Retrieve a point  
  TS_Point ts_p = _ts.getPoint();

  TS_Point tft_p;

  int box_x, box_y;
  
  // Serial.printf("X = %i\tY = %i\tPressure = %i\n", ts_p.x, ts_p.y, ts_p.z);  
  
  // Serial.printf("ts_p.x: %i\t ts_p.y: %i\n", ts_p.x, ts_p.y);
 

  // Scale from ~0->4000 to _tft.width using the calibration #'s
  // This was surprisingly confusing to adapt to the current rotation..
  tft_p.x = map(ts_p.y, TS_MINY, TS_MAXY, _tft.width(), 0);
  tft_p.y = map(ts_p.x, TS_MAXX, TS_MINX, 0, _tft.height());

  if (tft_p.x != lastTouch_x || tft_p.y != lastTouch_y) {

    if (!lastTouch_x && !lastTouch_y) {

    }

    lastTouch_x = tft_p.x;
    lastTouch_y = tft_p.y;

    // Serial.printf("tft_x: %i\t tft_y: %i\n", tft_p.x, tft_p.y);

    box_x = tft_p.x/TS_BUTTON_SIZE;
    box_y = tft_p.y/TS_BUTTON_SIZE;

    Serial.printf("box: %i,%i\n", box_x, box_y);
    
  }


  // paint(tft_p.x, tft_p.y);

};

void PbMonitor::hideGrid() {
  int numRows = _tft.height()/TS_BUTTON_SIZE;
  int numCols = _tft.width()/TS_BUTTON_SIZE;

  int boxcount=0, i, j, x1, x2, y1, y2, h, w;

  Serial.printf("rows: %i\tcols: %i\n", numRows, numCols);

  // each horizontal line
  for (i=1; i < numRows; i++) {

    x1 = 0;
    x2 = _tft.width();
    y1 = i*TS_BUTTON_SIZE;
    y2 = i*TS_BUTTON_SIZE;


    _tft.drawLine(x1, y1, x2, y2, HX8357_WHITE);
  }
  
  // each column
  for (j=1; j< numCols; j++, boxcount++) {

    x1 = j*TS_BUTTON_SIZE;
    x2 = j*TS_BUTTON_SIZE;
    y1 = 0;
    y2 = _tft.height();

    _tft.drawLine(x1, y1, x2, y2, HX8357_WHITE);

  }
};

void PbMonitor::showGrid() {
  int numRows = _tft.height()/TS_BUTTON_SIZE;
  int numCols = _tft.width()/TS_BUTTON_SIZE;

  int boxcount=0, i, j, x1, x2, y1, y2, h, w;

  Serial.printf("rows: %i\tcols: %i\n", numRows, numCols);

  // each horizontal line
  for (i=1; i < numRows; i++) {

    x1 = 0;
    x2 = _tft.width();
    y1 = i*TS_BUTTON_SIZE;
    y2 = i*TS_BUTTON_SIZE;


    _tft.drawLine(x1, y1, x2, y2, HX8357_BLACK);
  }
  
  // each column
  for (j=1; j< numCols; j++, boxcount++) {

    x1 = j*TS_BUTTON_SIZE;
    x2 = j*TS_BUTTON_SIZE;
    y1 = 0;
    y2 = _tft.height();

    _tft.drawLine(x1, y1, x2, y2, HX8357_BLACK);

  }
}


void PbMonitor::paint(int x, int y) {

  if (((y-PENRADIUS) > 0) && ((y+PENRADIUS) < _tft.height())) {
  _tft.fillCircle(x, y, PENRADIUS, currentcolor);
  }
};


bool PbMonitor::tstest() {
  uint16_t x, y;
  uint8_t z;
  if (_ts.touched()) {
    // read x & y & z;
    while (! _ts.bufferEmpty()) {
      Serial.print(_ts.bufferSize());
      _ts.readData(&x, &y, &z);
      Serial.print("->("); 
      Serial.print(x); Serial.print(", "); 
      Serial.print(y); Serial.print(", "); 
      Serial.print(z);
      Serial.println(")");
    }
    _ts.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
  }
  delay(10);
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
bool PbMonitor::setBackgroundImage(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x > _tft.width()) || (y > _tft.height())) {
    Serial.print("Images must not be larger than the TFT-LCD!");
    return false;
  }

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return false;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;



        if((x+w-1) >= _tft.width())  w = _tft.width()  - x;
        if((y+h-1) >= _tft.height()) h = _tft.height() - y;

        Serial.print("x: ");
        Serial.println(x);

        Serial.print("y: ");
        Serial.println(y);

        Serial.print("x+w-1");
        Serial.println(x+w-1);
        
        Serial.print("y+h-1");
        Serial.println(y+h-1);


        // Set TFT address window to clipped image bounds
        _tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...
          // Serial.print("row: ");
          // Serial.println(row);

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Serial.print("col: ");
            // Serial.println(col);

            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            _tft.pushColor(_tft.color565(r,g,b));
            // Serial.println("end pixel");
          } // end pixeL
          // Serial.println("end row");
          // delay(1000);
        } // end scanline
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();

  _tft.setAddrWindow(0,0,0,0);
  if(!goodBmp) {
    Serial.println(F("BMP format not recognized."));
    return false;
  } else {
    return true; 
  }
}







// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t PbMonitor::read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t PbMonitor::read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void PbMonitor::cardInfo() {
  uint8_t cardType =  SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  listDir(SD, "/", 0);

  uint64_t cardSize =  SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  Serial.printf("Total space: %lluMB\n",  SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n",  SD.usedBytes() / (1024 * 1024));


}

void PbMonitor::sdfstest(){

  cardInfo();
  
  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  removeDir(SD, "/mydir");
  listDir(SD, "/", 2);
  writeFile(SD, "/hello.txt", "Hello ");
  appendFile(SD, "/hello.txt", "World!\n");
  readFile(SD, "/hello.txt");
  deleteFile(SD, "/foo.txt");
  renameFile(SD, "/hello.txt", "/foo.txt");
  readFile(SD, "/foo.txt");
  testFileIO(SD, "/test.txt");

  cardInfo();

}

void PbMonitor::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void PbMonitor::createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void PbMonitor::removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

String PbMonitor::readFile(fs::FS &fs, const char * path){

  char chunk;

  String contents;

  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return "";
  }

  Serial.println("Read from file: ");

  while(file.available()){

    chunk = file.read();

    contents += chunk;
    // Serial.println(chunk);
    // Serial.println(contents);

  }

  contents.trim();

  Serial.println("complete contents: " + contents); //Printing for debugging purpose    
  file.close();


  return contents;
}

void PbMonitor::writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void PbMonitor::appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void PbMonitor::renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void PbMonitor::deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void PbMonitor::testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}
