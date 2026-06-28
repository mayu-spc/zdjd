// 两路LED引脚
const int ledA = 2;
const int ledB = 4;
// PWM参数
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  // 两路独立PWM初始化
  ledcAttach(ledA, freq, resolution);
  ledcAttach(ledB, freq, resolution);
}

void loop() {
  int duty;
  // A渐亮，B同步渐暗（反相关系）
  for (duty = 0; duty <= 255; duty++) {
    ledcWrite(ledA, duty);
    ledcWrite(ledB, 255 - duty);
    delay(8);
  }
  // A渐暗，B同步渐亮
  for (duty = 255; duty >= 0; duty--) {
    ledcWrite(ledA, duty);
    ledcWrite(ledB, 255 - duty);
    delay(8);
  }
} 