#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "esp_camera.h"
#include "ESP32FtpServer.h"

#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
//#include <EEPROM.h>            // read and write from flash memory

// define the number of bytes you want to access
//#define EEPROM_SIZE 2

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


const char* ssid = "SFR_EC58";
const char* password = "96wwza4yfz24qhtc4mxq";


FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial

#define FTP_DEBUG
//SD card options
/*
#define SD_CS 5
#define SDSPEED 40000000
*/

void setup(void){
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  Serial.println("");
  //pinMode(19, INPUT_PULLUP); //pullup GPIO2 for SD_MMC mode, you need 1-15kOm resistor connected to GPIO2 and GPIO19

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /////FTP Setup, ensure SD is started before ftp;  /////////
  
  ////if (SD.begin( SD_CS, SPI, SDSPEED)) {
  if (SD_MMC.begin()) {
      Serial.println("SD opened!");
      ftpSrv.begin("esp32","esp32");    //username, password for ftp.  set ports in ESP32FtpServer.h  (default 21, 50009 for PASV)
  }    
}

void loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   

}









// #include <Arduino.h>
// #include "esp_camera.h"
// #include "FS.h"                // SD Card ESP32
// #include "SD_MMC.h"            // SD Card ESP32
// #include "soc/soc.h"           // Disable brownour problems
// #include "soc/rtc_cntl_reg.h"  // Disable brownour problems
// #include "driver/rtc_io.h"
// //#include <EEPROM.h>            // read and write from flash memory

// // define the number of bytes you want to access
// //#define EEPROM_SIZE 2

// // Pin definition for CAMERA_MODEL_AI_THINKER
// #define PWDN_GPIO_NUM     32
// #define RESET_GPIO_NUM    -1
// #define XCLK_GPIO_NUM      0
// #define SIOD_GPIO_NUM     26
// #define SIOC_GPIO_NUM     27

// #define Y9_GPIO_NUM       35
// #define Y8_GPIO_NUM       34
// #define Y7_GPIO_NUM       39
// #define Y6_GPIO_NUM       36
// #define Y5_GPIO_NUM       21
// #define Y4_GPIO_NUM       19
// #define Y3_GPIO_NUM       18
// #define Y2_GPIO_NUM        5
// #define VSYNC_GPIO_NUM    25
// #define HREF_GPIO_NUM     23
// #define PCLK_GPIO_NUM     22


// // Delay time in millieconds
// unsigned int delayTime = 10;

// unsigned int PictureNumber_ByBatch = 10;

// RTC_DATA_ATTR int  pictureNumber = 0;

// void takeNewPhoto() {
//   // Take Picture with Camera
  
//   // Path where new picture will be saved in SD Card

//   pictureNumber++;
//   String path = "/picture" + String(pictureNumber) +".jpg";

//   Serial.println(path);

//   // Setup frame buffer
//   camera_fb_t  * fb = esp_camera_fb_get();


//   if (!fb) {
//     Serial.println("Camera capture failed");
//     return;
//   }

//   // Save picture to microSD card
//   fs::FS &fs = SD_MMC;
//   File file = fs.open(path.c_str(), FILE_WRITE);
//   if (!file) {
//     Serial.println("Failed to open file in write mode");
//   }
//   else {
//     file.write(fb->buf, fb->len); // payload (image), payload length
//     Serial.printf("Saved file to path: %s\n", path.c_str());

//   }
//   // Close the file
//   file.close();

//   // Return the frame buffer back to the driver for reuse
//   esp_camera_fb_return(fb);
// }

// void setup() {

//   // Start Serial Monitor
//   Serial.begin(115200);
//   Serial.println("Test");
  
//   WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

//   //Serial.setDebugOutput(true);
//   //Serial.println();
  
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG; 
  
//   if(psramFound()){
//     config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
//     config.jpeg_quality = 10;
//     config.fb_count = 2;
//   } else {
//     config.frame_size = FRAMESIZE_SVGA;
//     config.jpeg_quality = 12;
//     config.fb_count = 1;
//   }
  
//   // Init Camera
//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK) {
//     Serial.printf("Camera init failed with error 0x%x", err);
//     return;
//   }
  
//   //Serial.println("Starting SD Card");
//   if(!SD_MMC.begin()){
//     Serial.println("SD Card Mount Failed");
//     return;
//   }
  
//   uint8_t cardType = SD_MMC.cardType();
//   if(cardType == CARD_NONE){
//     Serial.println("No SD Card attached");
//     return;
//   }


// for (int i = 0; i < PictureNumber_ByBatch; i++) {


//   delay(delayTime);
//   takeNewPhoto();

// }

//   // Bind Wakeup to GPIO13 going HIGH
//   esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);
  
//   delay(2000);
//   Serial.println("Going to sleep now");
//   delay(2000);
//   esp_deep_sleep_start();
//   Serial.println("This will never be printed");
// }

// void loop() {
  
// }