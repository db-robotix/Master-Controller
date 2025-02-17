#ifndef ANADIGMASTER_H
#define ANADIGMASTER_H

// Library for analog and digital ports of Arduino Zero
// (C) db robotix

#include <Arduino.h>
#include "ServoSAMD.h"

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

private:
  const byte LedPin = 2;
  const byte LedPinInv = 3;
  const byte LSensorPin = A3;
  const byte RSensorPin = A4;
  const byte averaging = 10;
};

enum servoTypes {MINI, GEEK};

class ServoMotor : public Servo {
public:
  ServoMotor(byte _type);
  
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
  const byte servoPin = 8;
  int16_t maxAngle;
  uint16_t pw_min;
  uint16_t pw_max;
};

#endif