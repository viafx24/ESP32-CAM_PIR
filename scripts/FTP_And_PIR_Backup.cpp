#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "esp_camera.h"
#include "ESP32FtpServer.h"

#include "FS.h"               // SD Card ESP32
#include "SD_MMC.h"           // SD Card ESP32
#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"
// #include <EEPROM.h>            // read and write from flash memory

// define the number of bytes you want to access
// #define EEPROM_SIZE 2

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

const char *ssid = "SFR_EC58";
const char *password = "96wwza4yfz24qhtc4mxq";

const int GPIO_SWITCH = 16;

FtpServer ftpSrv; // set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial

#define FTP_DEBUG
// SD card options
/*
#define SD_CS 5
#define SDSPEED 40000000
*/

void setup(void)
{
  Serial.begin(115200);
  pinMode(GPIO_SWITCH, INPUT_PULLUP);

  // pinMode(19, INPUT_PULLUP); //pullup GPIO2 for SD_MMC mode, you need 1-15kOm resistor connected to GPIO2 and GPIO19

  int GPIO_12 = digitalRead(GPIO_SWITCH);

  Serial.println(GPIO_12);

  if (GPIO_12 == 0)
  {

    WiFi.begin(ssid, password);
    Serial.println("");
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
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
    if (SD_MMC.begin())
    {
      Serial.println("SD opened!");
      ftpSrv.begin("esp32", "esp32"); // username, password for ftp.  set ports in ESP32FtpServer.h  (default 21, 50009 for PASV)
    }
    while (GPIO_12 == 0)
    {
    ftpSrv.handleFTP();
    GPIO_12 = digitalRead(GPIO_SWITCH);
    }
  }

  // Bind Wakeup to GPIO13 going HIGH
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);

  delay(2000);
  Serial.println("Going to sleep now");
  delay(2000);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop(void)
{
  // make sure in loop you call handleFTP()!!
}