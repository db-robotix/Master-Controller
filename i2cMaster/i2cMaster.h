#ifndef I2CMASTER_H
#define I2CMASTER_H

// Library for I2C bus clients of Arduino Zero as master
// (C) db robotix

#include <Arduino.h>
#include <Wire.h>

enum motorSCommand { NONE, GO, STOP, SPEED, STEERING, ACCEL, DECEL, TARGET, COAST, BRAKE };  // do not change !
enum motorDCommand { NONE_X, GO_A, STOP_A, SPEED_A, ACCEL_A, DECEL_A, TARGET_A, COAST_A, BRAKE_A, GO_B, STOP_B, SPEED_B, ACCEL_B, DECEL_B, TARGET_B, COAST_B, BRAKE_B };  // do not change !

/********************************************************************************/
class Drivetrain {
public:
  Drivetrain(const uint8_t i2c_address);  // constructor
  
/**
 * @brief Send 3 bytes over I2C bus
 */
  void sendCommand(byte command, int16_t value);
  
/**
 * @brief Set acceleration and deceleration in cm/s2
 */
  void setAccelerations(int16_t accel, int16_t decel);
  
/**
 * @brief Set default values of acceleration and deceleration
 */
  void setAccelerations();
  
/**
 * @brief Set speed parameter in cm/s
 */
  void setSpeed(int16_t speed);
  
/**
 * @brief Set steering parameter -100 ... +100
 */
  void setSteering(int16_t steering);
  
/**
 * @brief Set motor steps to target
 */
  void setTargetSteps(int16_t steps);
  
/**
 * @brief Start motors
 */
  void go();
  
/**
 * @brief Stop motors
 */
  void stop();
  
/**
 * @brief Set motors in brake mode - needs current
 */
  void brake();
  
/**
 * @brief Set motors in coast mode - switch them off
 */
  void coast();
  
/**
 * @brief Get status word from motor control: steps left or -1 if stopped
 */
  int16_t getStatus();
  
/**
 * @brief Estimate the total running time in milliseconds
 */
  uint16_t estimateTime(int32_t distance, int16_t speed, int16_t accel, int16_t decel);

  int16_t Accel;
  int16_t Decel;
  const int16_t VMAX = 100;
  const int16_t ACCELMAX = 250;  // cm/s2  max 500
private:
  byte address;
};

/********************************************************************************/
class MotorsX {
public:
  MotorsX(const uint8_t i2c_address);  // constructor
  
/**
 * @brief Send 3 bytes over I2C bus
 */
  void sendCommand(byte command, int16_t value);
  
/**
 * @brief Set acceleration and deceleration for motor A in degrees/s2
 */
  void setAccelerations_A(int16_t accel, int16_t decel);
  
/**
 * @brief Set acceleration and deceleration for motor B in degrees/s2
 */
  void setAccelerations_B(int16_t accel, int16_t decel);
  
/**
 * @brief Set default values of acceleration and deceleration for both motors A and B
 */
  void setAccelerations();
  
/**
 * @brief Set speed parameter for motor A in degrees/s
 */
  void setSpeed_A(int16_t speed);
  
/**
 * @brief Set speed parameter for motor B in degrees/s
 */
  void setSpeed_B(int16_t speed);
  
/**
 * @brief Set motor A steps to target
 */
  void setTargetSteps_A(int16_t steps);
  
/**
 * @brief Set motor B steps to target
 */
  void setTargetSteps_B(int16_t steps);
  
/**
 * @brief Start motor A
 */
  void go_A();
  
/**
 * @brief Start motor B
 */
  void go_B();
  
/**
 * @brief Stop motor A
 */
  void stop_A();
  
/**
 * @brief Stop motor B
 */
  void stop_B();
  
/**
 * @brief Set both (!) motors in brake mode - needs current
 */
  void brake_A();
  
/**
 * @brief Set both (!) motors in brake mode - needs current
 */
  void brake_B();
  
/**
 * @brief Set both (!) motors in coast mode - switch them off
 */
  void coast_A();
  
/**
 * @brief Set both (!) motors in coast mode - switch them off
 */
  void coast_B();
  
/**
 * @brief Get status word from motor control: 0 = both off, +1 A on, +2 B on
 */
  int16_t getStatus();

  const int16_t ACCELMAX = 10000;  // deg/s2
private:
  byte address;
};

/********************************************************************************/
// OLED display:

#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

class Display : public SSD1306AsciiWire {
public:
  Display();  // constructor
  
/**
 * @brief Start display library
 */
  void start();
  
/**
 * @brief Set row 1 ... 4 for next print operation
 */
  void setRow(byte row);
};

/********************************************************************************/
// APDS 9960 color sensor:

#include <SparkFun_APDS9960.h>

enum colors { BLACK, RED, YELLOW, GREEN, BLUE, WHITE };  // do not change !
enum colors_de { SCHWARZ, ROT, GELB, GRUEN, BLAU, WEISS };  // do not change !

class ColorSensorA : public SparkFun_APDS9960 {
public:
  
/**
 * @brief Start color sensor library
 */
  void start();
  
/**
 * @brief Start color sensor library
 */
  void start(byte digPin);
  
/**
 * @brief Switch LEDs on
 */
  void ledOn();
  
/**
 * @brief Switch LEDs off
 */
  void ledOff();
  
/**
 * @brief Measure and write RGB values to the 3 variables in parenthesis
 */
  void getRGB(uint16_t &_r, uint16_t &_g, uint16_t &_b);
  
/**
 * @brief Measure and write RGB values to the instance variables r,g,b
 */
  void getRGB();
  
/**
 * @brief Differential measurement and write RGB values to the 3 variables in parenthesis
 */
  void flashRGB(uint16_t &_r, uint16_t &_g, uint16_t &_b);
  
/**
 * @brief Differential measurement and write RGB values to the instance variables r,g,b
 */
  void flashRGB();
  
/**
 * @brief Measure dark RGB values and store to variables r0,g0,b0
 */
  void calibrate();
  
/**
 * @brief set dark values r0,g0,b0 to zero
 */
  void reset();
  
/**
 * @brief Calculate the hue value (-179 ... +180 ; HSL color model) from the given RGB values
 */
  int16_t hue(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the hue value (-179 ... +180 ; HSL color model) from the last measurement
 */
  int16_t hue();
  
/**
 * @brief Calculate the color code (0 = none, 1 = red, 2 = yellow, 3 = green, 4 = blue) from the given RGB values
 */
  int16_t color(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the color code (0 = none, 1 = red, 2 = yellow, 3 = green, 4 = blue) from the last measurement
 */
  int16_t color();
  
/**
 * @brief Calculate the saturation (0 ... 100) from the given RGB values
 */
  int16_t saturation(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the saturation (0 ... 100) from the last measurement
 */
  int16_t saturation();

/**
 * @brief Calculate the reflection intensity from the given RGB values
 */
  int16_t intens(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the reflection intensity from the last measurement
 */
  int16_t intens();

  uint16_t r, g, b;
  uint16_t r0 = 0, g0 = 0, b0 = 0;  // dark values
  uint16_t blackLimit = 40;
private:
  byte ledPin;
};

/********************************************************************************/
// TCS 34725 color sensor:

#include "Adafruit_TCS34725.h"

class ColorSensorB : public Adafruit_TCS34725 {
public:
  
/**
 * @brief Start color sensor library
 */
  void start();
  
/**
 * @brief Measure and write RGB values to the 3 variables in parenthesis
 */
  void getRGB(uint16_t &_r, uint16_t &_g, uint16_t &_b);
  
/**
 * @brief Measure and write RGB values to the instance variables r,g,b
 */
  void getRGB();
  
/**
 * @brief Measure dark RGB values and store to variables r0,g0,b0
 */
  void calibrate();
  
/**
 * @brief set dark values r0,g0,b0 to zero
 */
  void reset();
  
/**
 * @brief Calculate the hue value (HSL color model) from the given RGB values
 */
  int16_t hue(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the hue value (HSL color model) from the last measurement
 */
  int16_t hue();
  
/**
 * @brief Calculate the color code (0 = none, 1 = red, 2 = yellow, 3 = green, 4 = blue) from the given RGB values
 */
  int16_t color(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the color code (0 = none, 1 = red, 2 = yellow, 3 = green, 4 = blue) from the last measurement
 */
  int16_t color();
  
/**
 * @brief Calculate the saturation (0 ... 100) from the given RGB values
 */
  int16_t saturation(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the saturation (0 ... 100) from the last measurement
 */
  int16_t saturation();

/**
 * @brief Calculate the reflection intensity from the given RGB values
 */
  int16_t intens(uint16_t _r, uint16_t _g, uint16_t _b);
  
/**
 * @brief Calculate the reflection intensity from the last measurement
 */
  int16_t intens();

  uint16_t r, g, b;
  uint16_t r0 = 0, g0 = 0, b0 = 0;  // dark values
};

#endif