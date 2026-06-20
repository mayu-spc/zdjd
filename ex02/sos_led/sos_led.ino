// 单LED引脚
const int ledPin = 4;

// millis时间记录变量
unsigned long currentTime = 0;
unsigned long lastTime = 0;

// SOS状态机枚举
enum SosState {
  STATE_S1,    // 第一段S：3短闪
  STATE_O,     // O：3长闪
  STATE_S2,    // 第二段S：3短闪
  STATE_PAUSE  // 一组SOS结束长停顿
};
SosState nowState = STATE_S1;

// 计时参数（单位ms，和你原代码时序一致）
const int flashShortOn  = 200;
const int flashShortOff = 200;
const int flashLongOn   = 600;
const int flashLongOff  = 200;
const int letterGap     = 500;
const int wordGap       = 2000;

// 计数变量
int flashCount = 0;
bool ledStatus = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  currentTime = millis();
  // 时间差判断，无阻塞执行
  if (currentTime - lastTime >= getCurrentInterval()) {
    lastTime = currentTime;
    runSosLogic();
  }
}

// 获取当前状态需要等待的时长
int getCurrentInterval() {
  switch(nowState) {
    case STATE_S1:
    case STATE_S2:
      return ledStatus ? flashShortOn : flashShortOff;
    case STATE_O:
      return ledStatus ? flashLongOn : flashLongOff;
    case STATE_PAUSE:
      return wordGap;
    default:
      return letterGap;
  }
}

// SOS核心状态流转逻辑
void runSosLogic() {
  // 停顿状态：等待完成后重置，从头循环SOS
  if (nowState == STATE_PAUSE) {
    Serial.println("===== 一组SOS发送完成，进入长停顿 =====");
    flashCount = 0;
    ledStatus = false;
    digitalWrite(ledPin, LOW);
    nowState = STATE_S1;
    return;
  }

  // 翻转LED亮灭
  ledStatus = !ledStatus;
  digitalWrite(ledPin, ledStatus ? HIGH : LOW);

  // LED熄灭时计数、切换状态
  if (!ledStatus) {
    flashCount++;
    // S段3次短闪完成，切换到O
    if ((nowState == STATE_S1 || nowState == STATE_S2) && flashCount >= 3) {
      flashCount = 0;
      delayMillis(letterGap); // 字母间隔等待（仅状态切换停顿，不阻塞主循环）
      if (nowState == STATE_S1) nowState = STATE_O;
      else nowState = STATE_PAUSE;
    }
    // O段3次长闪完成，切换到第二个S
    else if (nowState == STATE_O && flashCount >= 3) {
      flashCount = 0;
      delayMillis(letterGap);
      nowState = STATE_S2;
    }
  }
}

// 辅助：无阻塞等待指定毫秒（仅状态切换间隙用）
void delayMillis(unsigned long waitMs) {
  unsigned long start = millis();
  while(millis() - start < waitMs) {}
}