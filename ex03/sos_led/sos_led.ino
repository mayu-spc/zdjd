// ESP32板载LED引脚
const int ledPin = 4;

// millis计时变量
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

// SOS时序参数（单位ms）
const int shortOn  = 200;
const int shortOff = 200;
const int longOn   = 600;
const int longOff  = 200;
const int letterGap= 500;
const int wordGap  = 2000;

// 状态机：0=第一段S 1=O 2=第二段S 3=结束等待
int workStage = 0;
int flashCnt  = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= getCurrentInterval()) {
    previousMillis = currentMillis;
    runSosLogic();
  }
}

// 获取当前状态等待时长
int getCurrentInterval() {
  switch(workStage) {
    case 0: case 2: return ledState ? shortOn : shortOff;
    case 1: return ledState ? longOn : longOff;
    case 3: return wordGap;
    default: return letterGap;
  }
}

// 空函数，第三次提交补全逻辑
void runSosLogic() {

}