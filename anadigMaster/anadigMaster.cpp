#include "anadigMaster.h"

#define BatteryVoltagePin A0
float Battery::getVoltage() {
  uint16_t adcValue;
  analogReference(AR_INTERNAL2V23);
  delay(10);
  adcValue = analogRead(BatteryVoltagePin);
  return 0.0157 * adcValue;
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
  while(!pressed()) delay(1);  // wait until button pressed
  delay(2); // debouncing of button contact
  while(pressed()) delay(1);  // wait until button released
  delay(dly);
}
void Button::wait() {wait(0);}

uint16_t Button::count(uint8_t timeout = 2) {  // seconds
  uint16_t counts = 0;
  unsigned long timer = millis();
  while (millis() - timer < 1000*timeout) {
    if (pressed()) {
      counts++;
	  delay(5); // debouncing of button contact
	  while (pressed()) delay(1);  // wait until button released
	  timer = millis();
	}
	delay(5);
	}
  return counts;
}
uint16_t Button::count() {return count(2);}

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
    delayMicroseconds(50);
    a1 += analogReadFast(LSensorPin);
    a2 += analogReadFast(RSensorPin);
  }
  aL = constrain(a1 / averaging, 1, 1023);
  aR = constrain(a2 / averaging, 1, 1023);
}

void LineSensor::getReflections(int16_t &aL, int16_t &aR) {  // takes 2.5 millisec
  int32_t a1 = 0, a2 = 0;
  ledOn();
  for (int i = 0; i < averaging; i++) {
    delayMicroseconds(50);
    a1 += analogReadFast(LSensorPin);
    a2 += analogReadFast(RSensorPin);
  }
  ledOff();
  delay(1);
  for (int i = 0; i < averaging; i++) {
    delayMicroseconds(100);
    a1 -= analogReadFast(LSensorPin);
    a2 -= analogReadFast(RSensorPin);
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

void LineSensor::calibrate(int16_t _whiteL, int16_t _whiteR, int16_t _blackL, int16_t _blackR) {
  whiteL = _whiteL;
  whiteR = _whiteR;
  blackL = _blackL;
  blackR = _blackR;
}

int16_t LineSensor::getOffset() {
  int16_t a1, a2;
  int32_t left, right, diff;
  getReflections(a1, a2);
  left  = map(a1, blackL, whiteL, 0, 500);  // normalize to interval 0 ... 500
  left  = constrain(left, 0, 500);          // limit to interval 0 ... 500
  right = map(a2, blackR, whiteR, 0, 500);
  right = constrain(right, 0, 500);
  diff = right - left;                                          // standard case
  if (left > 450 && right < 450) diff = -(left + right);        // right sensor on left edge of line
  else if (left < 450 && right > 450) diff = left + right;      // left sensor on right edge of line
  else if (left > 450 && right > 450 && lastOffset > 0) diff = 1000;  // both sensors on white; consider history
  else if (left > 450 && right > 450 && lastOffset < 0) diff = -1000; // both sensors on white; consider history
  lastOffset = diff;
  return (int16_t)diff;
}

UltrasonicSensor::UltrasonicSensor() {  // constructor
  pinMode(triggerPin1, OUTPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  digitalWrite(triggerPin1, LOW);
  digitalWrite(triggerPin2, LOW);
}

int16_t UltrasonicSensor::measureDistance(uint8_t triggerPin, uint8_t echoPin) {
  uint32_t duration;  // us
  uint32_t distance;  // mm
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW); // start transmitting
  duration = pulseIn(echoPin, HIGH, 38000);  // max 38 ms
  distance = duration * ultrasoundSpeed / 2000; 
  if (distance > 2000) distance = 0;
  return (int16_t)distance;
}

int16_t UltrasonicSensor::getDistance() {
  return measureDistance(triggerPin1, echoPin1);
}

int16_t UltrasonicSensor::getDistance1() {
  return measureDistance(triggerPin1, echoPin1);
}

int16_t UltrasonicSensor::getDistance2() {
  return measureDistance(triggerPin2, echoPin2);
}

ServoMotor::ServoMotor(byte _type, byte _servoPin) {  // constructor
  lastAngle = 0;
  servoPin = _servoPin;
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