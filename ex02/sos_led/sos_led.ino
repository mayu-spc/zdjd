// 定义LED引脚，ESP32板载LED连接在GPIO 4
const int ledPin = 4;

// millis 计时变量（必须 unsigned long 防止溢出）
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

// SOS 时序参数 单位ms
const unsigned short shortOn  = 200;
const unsigned short shortOff = 200;
const unsigned short longOn   = 600;
const unsigned short longOff  = 200;
const unsigned short letterGap= 500;
const unsigned short endPause = 2000;

// 状态标记
bool ledState = false;
int flashCnt  = 0;
// 0=第一段S 1=O 2=第二段S 3=一轮结束等待
int workStage = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  currentMillis = millis();
  // 时间差判断，无阻塞执行
  if (currentMillis - previousMillis >= getCurrentInterval()) {
    previousMillis = currentMillis;
    runSosLogic();
  }
}

// 获取当前状态需要等待的时长
int getCurrentInterval() {
  switch(workStage) {
    case 0:
    case 2:
      return ledState ? shortOn : shortOff;
    case 1:
      return ledState ? longOn : longOff;
    case 3:
      return endPause;
    default:
      return letterGap;
  }
}

// SOS核心状态流转逻辑
void runSosLogic() {
  // 停顿状态：等待完成后重置，从头循环SOS
  if (workStage == 3) {
    Serial.println("===== 一组SOS发送完成，进入长停顿 =====");
    flashCnt = 0;
    ledState = false;
    digitalWrite(ledPin, LOW);
    workStage = 0;
    return;
  }

  // 翻转LED亮灭
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? HIGH : LOW);
}