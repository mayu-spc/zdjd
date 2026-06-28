// 引脚定义
const int touchPin = 4;
const int ledPin = 2;

// 触摸读取缓存
uint16_t touchVal;
// 触摸阈值，小于该值判定为触摸
const uint16_t touchThresh = 450;

// 自锁状态
bool ledState = false;
// 上一次触摸状态：false=未触摸 true=触摸
bool lastTouch = false;
// 软件防抖计时
unsigned long debounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long now = millis();
  touchVal = touchRead(touchPin);
  bool curTouch = (touchVal < touchThresh);

  // 防抖判定
  if ((now - debounceTime) > debounceDelay) {
    // 边缘检测：上次没触摸，本次触摸（按下瞬间）
    if (curTouch == true && lastTouch == false) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      debounceTime = now;
    }
  }
  lastTouch = curTouch;
}