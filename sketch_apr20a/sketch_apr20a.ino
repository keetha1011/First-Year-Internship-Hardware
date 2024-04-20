#include "Arduino.h"
#include "WiFi.h"
#include "esp_camera.h"
#include "soc/soc.h"           
#include "soc/rtc_cntl_reg.h"  
#include "driver/rtc_io.h"
#include <LittleFS.h>
#include <FS.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <time.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include <pthread.h>

//#define FILE_PHOTO_PATH "/photo.jpg"
//#define BUCKET_PHOTO "/data/photo.jpg"

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

// Credentials for Firebase Storage
#define API_KEY "AIzaSyDbZAfkcG4ydAzjzq4HPQaE8ChSI47zBZY"
#define USER_EMAIL "nnm23am025@nmamit.in"
#define USER_PASSWORD "h6AHG5qWECNS9CJ"
#define STORAGE_BUCKET_ID "internship-first-year.appspot.com"

// Credentials for Sinric Pro and Device location
#define APP_KEY          "3d3e0d6e-5cfd-4ccf-8304-854e3b6e77dc"
#define APP_SECRET        "34d65426-9531-4003-a672-66ba069cc182-f8d5ec25-1825-4a93-9f38-d3003bf4938c"
#define SWITCH_ID_1       "655f09ac69edf60a3764d813"
#define RELAYPIN_1        13
#define SWITCH_ID_2       "655dd4b169edf60a37641a8d"
#define RELAYPIN_2        14

#define PIR_PIN 16

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

time_t now = time(nullptr);
struct tm* timeinfo = localtime(&now);
char FILE_PHOTO_PATH[20];

char BUCKET_PHOTO[50];

boolean takeNewPhoto = true;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

String uid = "";

//void fcsUploadCallback(FCS_UploadStatusInfo info);

bool taskCompleted = false;

void setup() {
  
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);

  initWiFi();
  initLittleFS();
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  initCamera();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  configF.api_key = API_KEY;
  
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  configF.token_status_callback = tokenStatusCallback; 

  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);

  while (auth.token.uid == "") {
    delay(100);
  }

  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.print(uid);

  setupSinricPro();

  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, thread_security_camera, NULL);
  delay(10);
  pthread_create(&thread2, NULL, thread_SinricPro, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}

void loop() {

  if (Firebase.isTokenExpired()){
    Firebase.refreshToken(&configF);
    Serial.println("Refresh token");
  }

  delay(10000);
}

