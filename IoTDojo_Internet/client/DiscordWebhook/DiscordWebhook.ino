#include <WiFiClientSecure.h>

const char* ssid     = "ssid";
const char* password = "password";

const char* HOST = "discord.com";
const String URL = "/api/webhooks/hogehoge";

void Send(String mes) {
  WiFiClientSecure client;
  client.setInsecure();
  String cont = "{\"content\":\"" + mes + "\"}";
  if (!client.connect(HOST, 443)) {
    Serial.println("Not connect");
    return;
  }
  client.println("POST " + URL + " HTTP/1.1");
  client.println("Content-Length: " + String(cont.length()));
  client.println("Content-Type: application/json");
  client.println("Host: " + String(HOST));
  client.println("Connection: close");
  client.println();
  client.println(cont);
  Serial.println(cont);

  delay(100);
}
void setup() {
  pinMode(4,INPUT);
  
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Start");
}

void loop() {
  if (digitalRead(4)==HIGH) {
    Send("ボタンが押されました！");
  }
}
