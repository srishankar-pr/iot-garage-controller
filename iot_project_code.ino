#define BLYNK_TEMPLATE_ID "**********************"
#define BLYNK_TEMPLATE_NAME "Garage control system"
#define BLYNK_AUTH_TOKEN "************************"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// Wi-Fi credentials
char ssid[] = "your_ssid";
char pass[] = "your_password";

// Pin definitions
const int servoPin = 13;
const int buzzerPin = 26;
const int ledPin = 25;
const int trigPin = 14;
const int echoPin = 27;

// Servo object
Servo myServo;

// Ultrasonic sensor threshold
const int carThreshold = 10;  // Distance in cm

// Blynk virtual pins
#define V_SERVO_BUTTON V1
#define V_LED_CONTROL V2
#define V_CAR_PRESENCE V3

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(0);  // Start at 0 degrees
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;  // Distance in cm
}

BLYNK_WRITE(V_SERVO_BUTTON) {
  int buttonState = param.asInt();
  if (buttonState == 1) {
    myServo.write(90);
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
  } else {
    myServo.write(0);
  }
}

BLYNK_WRITE(V_LED_CONTROL) {
  int ledState = param.asInt();
  digitalWrite(ledPin, ledState);
}

void loop() {
  Blynk.run();

  int distance = measureDistance();
  if (distance <= carThreshold && distance > 0) {
    Blynk.virtualWrite(V_CAR_PRESENCE, "Car Present");
  } else {
    Blynk.virtualWrite(V_CAR_PRESENCE, "No Car");
  }

  delay(500);
}
