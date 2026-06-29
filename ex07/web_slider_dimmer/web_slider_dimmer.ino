#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "vivo S19 Pro";
const char* password = "mmmmmmmm";
const int LED_PIN = 2;

WebServer server(80);
int brightness = 0;

// 生成网页页面，和你示例统一用 makePage + rawliteral
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex07 网页无极滑动调光</title>
  <style>
    body {font-family:Arial; text-align:center; margin-top:80px; font-size:26px;}
    #slider {width:85%; height:32px; margin:30px 0;}
  </style>
</head>
<body>
  <h1>网页滑动条无极调光</h1>
  <!-- 作业要求 range 滑块 min0 max255 -->
  <input type="range" id="slider" min="0" max="255" value="0">
  <p>当前亮度数值：<span id="val">0</span></p>
  <script>
    const slider = document.getElementById("slider");
    const valText = document.getElementById("val");
    // 拖动实时发送请求
    slider.oninput = function(){
      let v = this.value;
      valText.innerText = v;
      fetch("/set?bri="+v);
    }
  </script>
</body>
</html>
)rawliteral";
  return html;
}

// 主页路由
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 接收滑块亮度参数
void handleSetBright() {
  if (server.hasArg("bri")) {
    brightness = server.arg("bri").toInt();
   ledcWrite(LED_PIN, brightness);
  }
  server.send(200, "text/plain; charset=UTF-8", "OK");
}

void setup() {
  Serial.begin(115200);
  // 新版PWM配置，兼容当前内核，无报错
  ledcAttach(LED_PIN, 5000, 8);
  ledcWrite(LED_PIN, 0);

  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  // 注册接口路由
  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
}

void loop() {
  server.handleClient();
}