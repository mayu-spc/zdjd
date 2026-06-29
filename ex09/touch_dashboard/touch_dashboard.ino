#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "vivo S19 Pro";
const char* password = "mmmmmmmm";
const int TOUCH_PIN = 4;

WebServer server(80);

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ex09 触摸实时仪表盘</title>
<style>
body{font-family:Arial;text-align:center;margin-top:100px;}
#num{font-size:90px;color:#0066cc;font-weight:bold;}
h2{font-size:32px;}
</style>
</head>
<body>
<h2>触摸传感器实时数值</h2>
<div id="num">0</div>
<script>
function refreshData(){
  fetch("/data")
  .then(res=>res.text())
  .then(val=>{
    document.getElementById("num").innerText = val;
  })
}
setInterval(refreshData, 200);
</script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleData() {
  uint16_t touchData = touchRead(TOUCH_PIN);
  server.send(200, "text/plain; charset=UTF-8", String(touchData));
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}