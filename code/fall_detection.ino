#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Wheelchair Safety"
#define BLYNK_AUTH_TOKEN YOUR_BLYNK_TOKEN

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <MPU6050.h>
#include <DFRobotDFPlayerMini.h>
#include <math.h>

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

MPU6050 mpu;

HardwareSerial mySerial(2);
DFRobotDFPlayerMini player;

#define BUZZER 25
#define SNOOZE 4
#define BUSY 27

bool alertActive = false;
bool emergencyMode = false;

unsigned long fallTime = 0;
unsigned long lastBeep = 0;

int audioStep = 0;

BlynkTimer timer;

BLYNK_CONNECTED() {
  Blynk.virtualWrite(V1,0);
}

void checkFall() {

  int16_t ax, ay, az;
  mpu.getAcceleration(&ax,&ay,&az);

  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  float accel = sqrt(ax_g*ax_g + ay_g*ay_g + az_g*az_g);

  float roll  = atan2(ay_g, az_g) * 180 / PI;
  float pitch = atan2(-ax_g, sqrt(ay_g*ay_g + az_g*az_g)) * 180 / PI;

  Serial.print("X:");
  Serial.print(ax_g);
  Serial.print(" Y:");
  Serial.print(ay_g);
  Serial.print(" Z:");
  Serial.print(az_g);
  Serial.print(" Roll:");
  Serial.print(roll);
  Serial.print(" Pitch:");
  Serial.println(pitch);

  Blynk.virtualWrite(V0, ax_g);
  Blynk.virtualWrite(V2, ay_g);
  Blynk.virtualWrite(V3, az_g);
  Blynk.virtualWrite(V4, roll);
  Blynk.virtualWrite(V5, pitch);

  if (accel > 2.0 && !alertActive && !emergencyMode) {

    Serial.println("Jerk detected");

    alertActive = true;
    fallTime = millis();

    Blynk.virtualWrite(V1,1);
  }

  if (alertActive && !emergencyMode) {

    if (millis() - lastBeep > 500) {
      digitalWrite(BUZZER,!digitalRead(BUZZER));
      lastBeep = millis();
    }

    if (digitalRead(SNOOZE) == LOW) {

      Serial.println("Alert cancelled");

      alertActive = false;
      digitalWrite(BUZZER,LOW);
      Blynk.virtualWrite(V1,0);
    }

    if (millis() - fallTime > 20000) {

      Serial.println("Emergency! Sending alert");

      Blynk.logEvent("fall_alert");

      alertActive = false;
      emergencyMode = true;

      digitalWrite(BUZZER,HIGH);

      audioStep = 0;
    }
  }

  if (emergencyMode) {

    digitalWrite(BUZZER,HIGH);

    if (audioStep == 0) {
      player.play(1);
      audioStep = 1;
    }

    else if (audioStep == 1 && digitalRead(BUSY) == HIGH) {
      player.play(2);
      audioStep = 2;
    }

    else if (audioStep == 2 && digitalRead(BUSY) == HIGH) {
      audioStep = 0;
    }
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(BUZZER,OUTPUT);
  pinMode(SNOOZE,INPUT_PULLUP);
  pinMode(BUSY,INPUT);

  digitalWrite(BUZZER,LOW);

  Wire.begin(21,22);
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected");
  }
  else {
    Serial.println("MPU6050 ERROR");
  }

  mySerial.begin(9600,SERIAL_8N1,16,17);

  if (player.begin(mySerial)) {
    player.volume(25);
    Serial.println("DFPlayer Ready");
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(500L, checkFall);
}

void loop() {

  Blynk.run();
  timer.run();

}
