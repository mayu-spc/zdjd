// ESP32板载LED引脚
const int ledPin = 4;

// millis计时变量
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

// SOS时序参数（单位ms）
const int shortOn  = 200;   // 短闪亮
const int shortOff = 200;   // 短闪灭
const int longOn   = 600;   // 长闪亮
const int longOff  = 200;   // 长闪灭
const int letterGap= 500;   // 字母间隔
const int wordGap  = 2000;  // 一组SOS结束后长停顿

// 状态机：0=第一段S(三短) 1=O(三长) 2=第二段S(三短) 3=结束等待
int workStage = 0;
int flashCnt  = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("SOS三短三长三短闪烁实验启动");
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= getCurrentInterval()) {
    previousMillis = currentMillis;
    runSosLogic();
  }
}

// 获取当前阶段对应的亮灭时长
int getCurrentInterval() {
  switch(workStage) {
    case 0: case 2: return ledState ? shortOn : shortOff;
    case 1: return ledState ? longOn : longOff;
    case 3: return wordGap;
    default: return letterGap;
  }
}

// SOS状态跳转核心逻辑
void runSosLogic() {
  // 一组SOS播放完毕，长停顿后重置从头循环
  if (workStage == 3) {
    Serial.println("===== 一组SOS信号发送完成 =====");
    flashCnt = 0;
    ledState = false;
    digitalWrite(ledPin, LOW);
    workStage = 0;
    return;
  }

  // 翻转LED亮灭
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? HIGH : LOW);

  // LED熄灭时计数，完成3次闪烁切换下一阶段
  if (!ledState) {
    flashCnt++;
    if (flashCnt >= 3) {
      flashCnt = 0;
      workStage++;
    }
  }
}