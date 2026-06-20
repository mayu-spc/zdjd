// 定义LED引脚，ESP32板载LED连接在GPIO 4
const int ledPin = 4;

// millis 计时变量（必须 unsigned long 防止溢出）
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  currentMillis = millis();
}