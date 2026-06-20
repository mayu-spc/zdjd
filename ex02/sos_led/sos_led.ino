// ESP32板载LED引脚
const int ledPin = 4;

// millis计时变量（必须用unsigned long防止溢出）
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

// 1Hz闪烁：亮500ms，灭500ms，周期1秒
const long interval = 500;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("1Hz LED闪烁实验开始");
}

void loop() {
  currentMillis = millis();

  // 非阻塞延时：不使用delay()，不阻塞其他代码执行
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  }
}