#include "anadigMaster.h"

#define BatteryVoltagePin A0
float Battery::getVoltage() {
  uint16_t adcValue;
  analogReference(AR_INTERNAL2V23);
  adcValue = analogRead(BatteryVoltagePin);
  return 0.0158 * adcValue;
}

uint16_t Battery::percent(float voltage) {
  return constrain( (int16_t)((voltage - 10.5) * 54), 0, 100 );  // 12.4 V = 100%
}

Button::Button() {  // constructor
  pinMode(ButtonPin, INPUT_PULLUP);
}

bool Button::pressed() {  // pin = LOW
  return !digitalRead(ButtonPin);
}

void Button::wait(uint32_t dly = 0) {  // millisecs
  while(digitalRead(ButtonPin)) delay(1);  // wait until low = button pressed
  delay(2); // debouncing of button contact
  while(!digitalRead(ButtonPin)) delay(1);  // wait until high again
  delay(dly);
}

Led::Led() {  // constructor
  pinMode(LedPin, OUTPUT);
}

void Led::on() {
  digitalWrite(LedPin, HIGH);
}

void Led::off() {
  digitalWrite(LedPin, LOW);
};

void Led::blink(uint8_t count, uint16_t period) {  // period in ms
  for (int i = 0; i < count; i++) {
    on();
    delay(period/2);
    off();
    delay(period/2);
  }
}

LineSensor::LineSensor() {  // constructor
  pinMode(LedPin, OUTPUT);
  pinMode(LedPinInv, OUTPUT);
  ledOff();
}

void LineSensor::ledOn() {
  digitalWrite(LedPin, HIGH);
  digitalWrite(LedPinInv, LOW);
}

void LineSensor::ledOff() {
  digitalWrite(LedPin, LOW);
  digitalWrite(LedPinInv, HIGH);
}

void LineSensor::getAmbient(int16_t &aL, int16_t &aR) {
  int32_t a1 = 0, a2 = 0;
  for (int i = 0; i < averaging; i++) {
    a1 += analogRead(LSensorPin);
    a2 += analogRead(RSensorPin);
    delayMicroseconds(100);
  }
  aL = constrain(a1 / averaging, 1, 1023);
  aR = constrain(a2 / averaging, 1, 1023);
}

void LineSensor::getReflections(int16_t &aL, int16_t &aR) {
  int32_t a1 = 0, a2 = 0;
  ledOn();
  for (int i = 0; i < averaging; i++) {
    a1 += analogRead(LSensorPin);
    a2 += analogRead(RSensorPin);
    delayMicroseconds(100);
  }
  ledOff();
  delay(1);
  for (int i = 0; i < averaging; i++) {
    a1 -= analogRead(LSensorPin);
    a2 -= analogRead(RSensorPin);
    delayMicroseconds(100);
  }
  aL = constrain(a1 / averaging, 1, 1023);
  aR = constrain(a2 / averaging, 1, 1023);
}

int16_t LineSensor::getDiff() {
  int16_t a1, a2;
  getReflections(a1, a2);
  return a1 - a2;
}

int16_t LineSensor::getAmbientDiff() {
  int16_t a1, a2;
  getAmbient(a1, a2);
  return a1 - a2;
}

int16_t LineSensor::getSum() {
  int16_t a1, a2;
  getReflections(a1, a2);
  return a1 + a2;
}

int16_t LineSensor::getAmbientSum() {
  int16_t a1, a2;
  getAmbient(a1, a2);
  return a1 + a2;
}

ServoMotor::ServoMotor(byte _type) {  // constructor
  lastAngle = 0;
  switch (_type) {
    case MINI:
	  maxAngle = 180;
	  pw_min = 560;
	  pw_max = 2490;
	break;
	case GEEK:
	  maxAngle = 360;
	  pw_min = 510;
	  pw_max = 2512;
	break;
  }
}

int16_t ServoMotor::angle2pulsewidth(int16_t angle) {
  angle = constrain(angle, 0, maxAngle);
  return map(angle, 0, maxAngle, pw_min, pw_max);
}

void ServoMotor::turnTo(int16_t angle) {
  attach(servoPin);
  writeMicroseconds(angle2pulsewidth(angle));
  lastAngle = angle;
}

void ServoMotor::slowTo(int16_t angle, uint16_t speed) {  // speed in degrees/sec
  int16_t _angle = constrain(angle, 0, maxAngle);
  uint16_t dly = 1000 / speed;
  attach(servoPin);
  if (angle > lastAngle) {
    for (int16_t a = lastAngle; a <= angle; a ++) {
      writeMicroseconds(angle2pulsewidth(a));
      delay(dly);
    }
  }
  else if (angle < lastAngle) {
    for (int16_t a = lastAngle; a >= angle; a --) {
      writeMicroseconds(angle2pulsewidth(a));
      delay(dly);
    }
  }
  lastAngle = angle;
}

void ServoMotor::coast() {
  delay(100);
  detach();
}