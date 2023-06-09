#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ssid";
const char* password = "password";

const char* rootCACertificate = "-----BEGIN CERTIFICATE-----\nなんかいっぱいの文字列\n-----END CERTIFICATE-----\n";
const String URL = "https://hooks.slack.com/services/hogehoge";

const int button = 4;

void setup() {
  pinMode(button, INPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Start");
}

void loop() {
  if (digitalRead(4) == HIGH) {
    HTTPClient https;  //HTTPリクエスト用の変数
    if (https.begin(URL, rootCACertificate)) {
      char* json = "{\"username\":\"フリーマン\",\"text\":\"自由なテキスト\"}";
      Serial.print("サーバに送るテキスト:");
      Serial.println(json);

      https.addHeader("Content-Type", "application/json");

      int httpCode = https.POST((uint8_t*)json, strlen(json));  //ここでHTTPリクエストを送っている
      if (httpCode > 0) {
        Serial.printf("ステータスコード: %d\n", httpCode);

        String payload = https.getString();
        Serial.println(payload);
      } else {
        Serial.printf("リクエストが送れませんでした: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    }
  }
}
