#define BLYNK_TEMPLATE_ID "TMPL64JPUmG_q"
#define BLYNK_TEMPLATE_NAME "SmartLightUsing PIR"
#define BLYNK_AUTH_TOKEN "u9AlKF926oLVt1RTbt1JcW0GdcF0Jm3L"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int relay1 = 22;
const int motionsensor = 34;
int pinStateCurrent = LOW;
int pinStatePrevious = LOW;

bool autoMode = true;              // true: dùng cảm biến, false: điều khiển tay
bool manualLightState = false;     // lưu trạng thái đèn khi ở chế độ thủ công

BlynkTimer timer;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);  // trạng thái đèn
  Blynk.syncVirtual(V1);  // trạng thái mô tả
  Blynk.syncVirtual(V2);  // chế độ hoạt động
  Blynk.syncVirtual(V3);  // bật/tắt đèn bằng tay
}

BLYNK_WRITE(V2) {
  int mode = param.asInt();
  autoMode = (mode == 0); // 0 = Auto, 1 = Manual

  if (autoMode) {
    Blynk.virtualWrite(V1, "Chế độ: Tự động");
  } else {
    Blynk.virtualWrite(V1, "Chế độ: Thủ công");
  }
}

BLYNK_WRITE(V3) {
  int state = param.asInt();

  if (!autoMode) {
    if (state == 1) {
      digitalWrite(relay1, LOW);  // bật đèn
      Blynk.virtualWrite(V0, 1);
      Blynk.virtualWrite(V1, "Đèn bật (Thủ công)");
      manualLightState = true;
    } else {
      digitalWrite(relay1, HIGH); // tắt đèn
      Blynk.virtualWrite(V0, 0);
      Blynk.virtualWrite(V1, "Đèn tắt (Thủ công)");
      manualLightState = false;
    }
  } else {
    // Nếu đang ở chế độ tự động, không cho phép điều khiển tay
    Blynk.virtualWrite(V1, "Đổi sang chế độ Thủ công để điều khiển");
    Blynk.virtualWrite(V3, manualLightState ? 1 : 0);  // reset trạng thái nút
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, "Lam", "Nganngo89@");

  pinMode(motionsensor, INPUT);
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH); // tắt đèn lúc khởi động
}

void loop() {
  Blynk.run();
  timer.run();

  if (autoMode) {
    pinStatePrevious = pinStateCurrent;
    pinStateCurrent = digitalRead(motionsensor);

    if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {
      Serial.println("Motion detected!");
      digitalWrite(relay1, LOW);
      Blynk.virtualWrite(V0, 1);
      Blynk.virtualWrite(V1, "Motion Detect");
    } else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {
      Serial.println("Motion stopped!");
      digitalWrite(relay1, HIGH);
      Blynk.virtualWrite(V0, 0);
      Blynk.virtualWrite(V1, "Motion Stopped");
    }
  }
}
