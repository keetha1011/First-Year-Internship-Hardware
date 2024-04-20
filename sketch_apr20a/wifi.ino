const char* ssid = "JACKEDGALAXYA34";
const char* password = "kmk123kmk";

void initWiFi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}
