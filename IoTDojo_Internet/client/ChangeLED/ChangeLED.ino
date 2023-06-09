#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ssid";
const char* password = "password";

const String id = "機器のID(自由)";
const String URL = "http://サーバのPCのIPアドレス/data/" + id;

const int led = 5;

void setup() {
  pinMode(led, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Start");
}

void loop() {
  HTTPClient http;  //HTTPリクエスト用の変数
  if (http.begin(URL)) {
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.GET();  //ここでHTTPリクエストを送っている
    if (httpCode > 0) {
      Serial.printf("ステータスコード: %d\n", httpCode);

      String payload = http.getString();
      Serial.println(payload);
      if(payload=="ON"){
        digitalWrite(led,HIGH);
      }else{
        digitalWrite(led,LOW);
      }
    } else {
      Serial.printf("リクエストが送れませんでした: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  delay(1000);
}
