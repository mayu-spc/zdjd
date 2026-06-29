#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "vivo S19 Pro";
const char* password = "mmmmmmmm";
const int LED_PIN = 2;
const int TOUCH_PIN = 4;
const uint16_t TOUCH_THRESH = 800;

WebServer server(80);
bool isArm = false;    // 全局布防状态
bool isAlarm = false;  // 全局报警锁定状态
bool lastTouch = false;
unsigned long debounceTime = 0;
const unsigned long debounceDelay = 20;

String makePage() {
  String tipTxt;
  if(!isArm) tipTxt = "系统未布防";
  else if(isAlarm) tipTxt = "⚠️ 报警触发锁定！";
  else tipTxt = "已布防，正常监控";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ex08 安防报警主机</title>
<style>
body{font-family:Arial;text-align:center;margin-top:80px;font-size:26px;}
button{font-size:28px;padding:15px 30px;margin:20px;}
#tip{color:red;font-weight:bold;}
</style>
</head>
<body>
<h1>物联网安防报警系统</h1>
<p id="tip">)"rawliteral"+tipTxt+R"rawliteral(</p>
<a href="/arm"><button>布防 ARM</button></a>
<a href="/disarm"><button>撤防 DISARM</button></a>
<script>
setInterval(()=>{
  fetch("/state").then(res=>res.text()).then(t=>{
    document.getElementById("tip").innerText = t;
  })
},500);
</script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}
// 布防接口
void handleArm(){
  isArm = true;
  server.sendHeader("Location", "/");
  server.send(303);
}
// 撤防接口，清除报警
void handleDisarm(){
  isArm = false;
  isAlarm = false;
  ledcWrite(LED_PIN, 255);
  server.sendHeader("Location", "/");
  server.send(303);
}
// 获取系统状态
void handleState(){
  String msg;
  if(!isArm) msg = "系统未布防";
  else if(isAlarm) msg = "⚠️ 报警触发锁定！";
  else msg = "已布防，正常监控";
  server.send(200, "text/plain; charset=UTF-8", msg);
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);
  ledcWrite(LED_PIN, 255);

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
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/state", handleState);
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long now = millis();
  uint16_t touchVal = touchRead(TOUCH_PIN);
  bool curTouch = touchVal < TOUCH_THRESH;

  // 仅布防且无报警时，触摸触发永久报警
  if(isArm && !isAlarm){
    if(now - debounceTime > debounceDelay){
      if(curTouch && !lastTouch){
        isAlarm = true;
        debounceTime = now;
      }
    }
  }
  lastTouch = curTouch;

  // 无阻塞高频闪烁报警，不卡住网页
  static unsigned long alarmTimer = 0;
  static bool ledFlip = false;
  if(isAlarm){
    if(now - alarmTimer >= 80){
      alarmTimer = now;
      ledFlip = !ledFlip;
      ledcWrite(LED_PIN, ledFlip ? 0 : 255);
    }
  }else{
    ledcWrite(LED_PIN, 255);
  }
}