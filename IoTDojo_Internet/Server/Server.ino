#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "ssid";
const char* password = "password";

const char* hostname = "free";

const int LED = 5;
bool state = HIGH;

WebServer server(80);

void mainPage() {
  if (server.method() == HTTP_GET) {
    server.send(200, "text/html","<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n  <meta charset=\"UTF-8\">\n  <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n  <title>IoT Dojo</title>\n  <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\"\n    integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\">\n</head>\n\n<body>\n\n  <!-- header -->\n  <header>\n    <div class=\"navbar navbar-dark bg-dark shadow-sm\">\n      <div class=\"container\">\n        <div class=\"navbar-brand d-flex align-items-center\">\n          <strong>AizuHack</strong>\n        </div>\n      </div>\n    </div>\n  </header>\n\n  <main class=\"container\">\n\n    <section>\n      <div class=\"text-center\">\n        <p class=\"led-state\">\n          現在の状態: 点灯\n        </p>\n      </div>\n    </section>\n\n    <section>\n      <h3>Lチカボタン</h3>\n      <br>\n      <div class=\"row\">\n        <div class=\"text-center\">\n          <button class=\"btn btn-primary\" onclick=\"ChangeLEDState('on')\">点灯</button>\n          <button class=\"btn btn-danger\" onclick=\"ChangeLEDState('off')\">消灯</button>\n        </div>\n      </div>\n\n      </div>\n    </section>\n\n  </main>\n\n</body>\n\n<script src=\"https://unpkg.com/axios/dist/axios.min.js\"></script>\n\n</html>\n\n<style>\n  main {\n    padding-top: 50px;\n  }\n\n  h3 {\n    padding: 1rem 2rem;\n    border-left: 5px solid #000;\n    background: #f4f4f4;\n  }\n\n  button {\n    width: 120px;\n    height: 80px;\n    margin-bottom: 10px;\n    margin-right: 20px;\n    margin-left: 20px;\n    font-size: 1.5rem !important;\n  }\n\n  .led-state {\n    font-size: 1.8rem;\n  }\n</style>\n\n<script>\n  const ChangeLEDState = (new_state) => {\n    const request = new XMLHttpRequest();\n    request.open('POST', '/state', false);\n    request.setRequestHeader(\"Content-Type\", \"text/plain\");\n    request.send(new_state);\n\n    if (request.status === 200) {\n      alert('リクエストに成功しました');\n    }\n  }\n\n  const GetLEDState = () => {\n    const request = new XMLHttpRequest();\n    request.open('GET', '/state', false);\n    request.send(null);\n\n    if (request.status === 200) {\n      const state = request.responseText;\n      const ledState = document.querySelector('.led-state');\n      ledState.innerText = `現在の状態: ${state}`;\n    }\n  }\n  setInterval(GetLEDState, 5000);\n\n</script>");
  } else {
    server.send(400, "text/plain", "GETリクエストを送ってください");
  }
}

void stateRequest() {
  if (server.method() == HTTP_GET) {
    String state_str;
    if (state == HIGH) {
      state_str = "ON";
    } else {
      state_str = "OFF";
    }

    server.send(200, "text/plain", state_str);
  } else if (server.method() == HTTP_POST) {
    String mes = server.arg("plain");

    if (mes == "on") {
      state = HIGH;
    } else if (mes == "off") {
      state = LOW;
    } else {
      server.send(400, "text/plain", "送る文字列はonまたはoffのみです");
      return;
    }
    server.send(200, "text/plain", "ok");

    digitalWrite(LED, state);
  } else {
    server.send(400, "text/plain", "GETまたはPOSTでリクエストを送ってください");
  }
}

void setup(void) {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  MDNS.begin(hostname);

  server.on("/", mainPage);
  server.on("/state", stateRequest);
  server.begin();
}

void loop(void) {
  server.handleClient();
}
