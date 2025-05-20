/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL64JPUmG_q"
#define BLYNK_TEMPLATE_NAME "SmartLightUsing PIR"
#define BLYNK_AUTH_TOKEN "u9AlKF926oLVt1RTbt1JcW0GdcF0Jm3L"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int relay1 = 22;
const int motionsensor = 34;   // the pin that OUTPUT pin of sensor is connected to
int pinStateCurrent   = LOW;   // current state of pin
int pinStatePrevious  = LOW;   // previous state of pin

BlynkTimer timer;

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);  // will cause BLYNK_WRITE(V0) to be executed
  Blynk.syncVirtual(V1);  // will cause BLYNK_WRITE(V1) to be executed
}

void setup() {
  // Debug console
  Serial.begin(115200);

  // Nhập thẳng tên Wi-Fi và mật khẩu vào đây
  Blynk.begin(BLYNK_AUTH_TOKEN, "Lam", "Nganngo89@");

  pinMode(motionsensor, INPUT); // set pin to input mode to read value from PIR
  pinMode(relay1, OUTPUT);      // output pin for relay
  digitalWrite(relay1, HIGH);  // Tắt relay lúc khởi động

}

void loop() {
  Blynk.run();
  timer.run();

  pinStatePrevious = pinStateCurrent;
  pinStateCurrent = digitalRead(motionsensor);

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {
    Serial.println("Motion detected!");
    digitalWrite(relay1, LOW);
    Blynk.virtualWrite(V0, 1);
    Blynk.virtualWrite(V1, "Motion Detected");
  } else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {
    Serial.println("Motion stopped!");
    digitalWrite(relay1, HIGH);
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, "Motion Stopped");
  }
}
