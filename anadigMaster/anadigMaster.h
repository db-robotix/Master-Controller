#ifndef ANADIGMASTER_H
#define ANADIGMASTER_H

// Library for analog and digital ports of Arduino Zero
// (C) db robotix

#include <Arduino.h>
#include "ServoSAMD.h"
#include "avdweb_AnalogReadFast.h"


class Battery {
public:
  
/**
 * @brief Measure the battery voltage in volts
 */
  float getVoltage();
  
/**
 * @brief Calculate battery capacity in percent from voltage
 */
  uint16_t percent(float voltage);
};

class Button {
public:
  Button();
  
/**
 * @brief Return TRUE if button pressed, and FALSE if not
 */
  bool pressed();
  
/**
 * @brief Wait until button was pressed and released plus dly milliseconds
 */
  void wait(uint32_t dly);
  void wait();  // default dly 0

/**
 * @brief Count button ticks with timeout in seconds
 */
  uint16_t count(uint8_t timeout);
  uint16_t count();  // default timeout 2 sec
  
private:
  const byte ButtonPin = 7;
};

class Led {
public:
  Led();
  
/**
 * @brief Turn LED on
 */
  void on();
  
/**
 * @brief Turn LED off
 */
  void off();
  
/**
 * @brief Make LED blink count times for period milliseconds
 */
  void blink(uint8_t count, uint16_t period);

private:
  const byte LedPin = 6;
};

class LineSensor {
public:
  LineSensor();
  
/**
 * @brief Switch LED on
 */
  void ledOn();
  
/**
 * @brief Switch LED off
 */
  void ledOff();
  
/**
 * @brief Measure ambient light of both sensors and write values to variables in parenthesis
 */
  void getAmbient(int16_t &a1, int16_t &a2);
  
/**
 * @brief Measure reflection of both sensors and write values to variables in parenthesis
 */
  void getReflections(int16_t &a1, int16_t &a2);
  
/**
 * @brief Measure reflections and return difference
 */
  int16_t getDiff();
  
/**
 * @brief Measure ambient light and return difference
 */
  int16_t getAmbientDiff();
  
/**
 * @brief Measure reflections and return sum
 */
  int16_t getSum();

/**
 * @brief Measure ambient light and return sum
 */
  int16_t getAmbientSum();

/**
 * @brief Set calibration values for white and black reflections of left and right sensor
 */
void calibrate(int16_t _whiteL, int16_t _whiteR, int16_t _blackL, int16_t _blackR);

/**
 * @brief Measure reflections and calculate offset from black line, max +/-1000 (neg = left, pos = right)
 */
int16_t getOffset();

private:
  const byte LedPin = 2;
  const byte LedPinInv = 3;
  const byte LSensorPin = A3;
  const byte RSensorPin = A4;
  const byte averaging = 5;
  int16_t whiteL;
  int16_t whiteR;
  int16_t blackL;
  int16_t blackR;
  int32_t lastOffset;
};

class UltrasonicSensor {
public:
  UltrasonicSensor();
  
/**
 * @brief Measure distance in mm, 0 = not valid
 */
  int16_t getDistance();
  int16_t getDistance1();
  int16_t getDistance2();

  uint16_t ultrasoundSpeed = 343;  // m/s
  
private:
  int16_t measureDistance(uint8_t triggerPin, uint8_t echoPin);
  const byte triggerPin1 = 4;
  const byte echoPin1 = 5;
  const byte triggerPin2 = 15;
  const byte echoPin2 = 16;
};


enum servoTypes {MINI, GEEK};

class ServoMotor : public Servo {
public:
  ServoMotor(byte _type, byte _servoPin);  // _servoPin = 8 or 9
  
/**
 * @brief Turn servo to degrees (absolutely) fast
 */
  void turnTo(int16_t angle);
  
/**
 * @brief Turn servo to degrees (absolutely) slowly with speed degrees/s
 */
  void slowTo(int16_t angle, uint16_t speed);
  
/**
 * @brief Let servo coast - turn current off
 */
  void coast();

private:
  int16_t angle2pulsewidth(int16_t angle);
  int16_t lastAngle;
  byte servoPin;  // 8 or 9
  int16_t maxAngle;
  uint16_t pw_min;
  uint16_t pw_max;
};

#endif