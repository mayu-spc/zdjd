const int touchPin = 4;
const int ledPin = 2;
const int freq = 5000;
const int resolution = 8;

// 触摸配置
uint16_t touchVal;
const uint16_t touchThresh = 800;
bool lastTouch = false;
unsigned long debounceTime = 0;
const unsigned long debounceDelay = 20;

// 3档速度，步长越大呼吸越快
int level = 1;
int stepArr[] = {1, 4, 8};

// 非阻塞呼吸变量，消除delay阻塞导致触摸迟钝
int duty = 0;
int dir = 1;
unsigned long breathTimer = 0;
const unsigned long breathInterval = 10;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
}

void loop() {
  unsigned long now = millis();
  uint16_t rawTouch = touchRead(touchPin);
  bool curTouch = rawTouch < touchThresh;

  // 触摸档位切换（复用ex04边缘检测防抖逻辑）
  if (now - debounceTime > debounceDelay) {
    if (curTouch && !lastTouch) {
      level++;
      if (level > 3) level = 1;
      debounceTime = now;
      Serial.print("当前呼吸档位：");
      Serial.println(level);
    }
  }
  lastTouch = curTouch;

  // 无阻塞呼吸渐变
  int step = stepArr[level - 1];
  if (now - breathTimer >= breathInterval) {
    breathTimer = now;
    duty += dir * step;
    if (duty >= 255) dir = -1;
    if (duty <= 0) dir = 1;
    ledcWrite(ledPin, duty);
  }
}