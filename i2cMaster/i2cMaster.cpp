#include "i2cMaster.h"

Drivetrain::Drivetrain(const uint8_t i2c_address) {
  // initialise
  address = i2c_address;
}

void Drivetrain::sendCommand(const uint8_t command, const int16_t value) {
  Wire.beginTransmission(address); // transmit to device
  Wire.write(command);
  Wire.write(lowByte(value));
  Wire.write(highByte(value));
  if (Wire.endTransmission()) { // stop transmitting 3 bytes and get error code
    Serial.println("Error on I2C transmission");
  }
  delay(1);
}

void Drivetrain::setAccelerations(int16_t accel, int16_t decel) {
  sendCommand(ACCEL, abs(accel*20));
  sendCommand(DECEL, abs(decel*20));
  Accel = accel;
  Decel = decel;
}

void Drivetrain::setAccelerations() {
  sendCommand(ACCEL, ACCELMAX *20);
  sendCommand(DECEL, ACCELMAX *20);
  Accel = ACCELMAX;
  Decel = ACCELMAX;
}

void Drivetrain::setSpeed(int16_t speed) {
  sendCommand(SPEED, speed *20);
}

void Drivetrain::setSteering(int16_t steering) {
  sendCommand(STEERING, steering);
}

void Drivetrain::setTargetSteps(int16_t steps) {
  sendCommand(TARGET, steps);
}

void Drivetrain::go() {
  sendCommand(GO, 0);
  delay(1);
  getStatus();  // avoid initial error
}

void Drivetrain::stop() {
  sendCommand(STOP, 0);
}

void Drivetrain::brake() {
  sendCommand(BRAKE, 0);
}

void Drivetrain::coast() {
  sendCommand(COAST, 0);
}

int16_t Drivetrain::getStatus() {
  Wire.requestFrom(address, 2);    // request 2 bytes from peripheral device #4
  // delay(1);
  uint8_t lo, hi;
  int16_t value = -99;
  if (Wire.available() == 2) {
    lo = Wire.read();    // receive low byte
    hi = Wire.read();    // receive high byte
    value = ((0x0000 | hi) << 8) | (0x0000 | lo);
  }
  else value = -9;  // error code
  return value;
}

uint16_t Drivetrain::estimateTime(int32_t distance, int16_t speed, int16_t accel, int16_t decel) {  // distance in mm, speed in cm/s, accel in cm/s2
  int16_t v_max = (int16_t)sqrt(0.2 * abs(distance) * accel * decel / (accel + decel));
  if (speed > v_max) speed = (int32_t)v_max;
  return 10 + abs(100 * distance / speed) + abs(1000 * speed / accel / 2) + abs(1000 * speed / decel / 2);
}

// ------------------------------

MotorsX::MotorsX(const uint8_t i2c_address) {
  // initialise
  address = i2c_address;
}

void MotorsX::sendCommand(const uint8_t command, const int16_t value) {
  Wire.beginTransmission(address); // transmit to device
  Wire.write(command);
  Wire.write(lowByte(value));
  Wire.write(highByte(value));
  if (Wire.endTransmission()) { // stop transmitting 3 bytes and get error code
    Serial.println("Error on I2C transmission");
  }
  delay(1);
}

void MotorsX::setAccelerations_A(int16_t accel, int16_t decel) {
  sendCommand(ACCEL_A, abs(accel*10/9));
  sendCommand(DECEL_A, abs(decel*10/9));
}

void MotorsX::setAccelerations_B(int16_t accel, int16_t decel) {
  sendCommand(ACCEL_B, abs(accel*10/9));
  sendCommand(DECEL_B, abs(decel*10/9));
}

void MotorsX::setAccelerations() {
  sendCommand(ACCEL_A, ACCELMAX/2 *10/9);
  sendCommand(DECEL_A, ACCELMAX/2 *10/9);
  sendCommand(ACCEL_B, ACCELMAX/2 *10/9);
  sendCommand(DECEL_B, ACCELMAX/2 *10/9);
}

void MotorsX::setSpeed_A(int16_t speed) {
  sendCommand(SPEED_A, speed *10/9);
}

void MotorsX::setSpeed_B(int16_t speed) {
  sendCommand(SPEED_B, speed *10/9);
}

void MotorsX::setTargetSteps_A(int16_t steps) {
  sendCommand(TARGET_A, steps);
}

void MotorsX::setTargetSteps_B(int16_t steps) {
  sendCommand(TARGET_B, steps);
}

void MotorsX::go_A() {
  sendCommand(GO_A, 0);
  delay(1);
  getStatus();  // avoid initial error
}

void MotorsX::go_B() {
  sendCommand(GO_B, 0);
  delay(1);
  getStatus();  // avoid initial error
}

void MotorsX::stop_A() {
  sendCommand(STOP_A, 0);
}

void MotorsX::stop_B() {
  sendCommand(STOP_B, 0);
}

void MotorsX::brake_A() {
  sendCommand(BRAKE_A, 0);
}

void MotorsX::brake_B() {
  sendCommand(BRAKE_B, 0);
}

void MotorsX::coast_A() {
  sendCommand(COAST_A, 0);
}

void MotorsX::coast_B() {
  sendCommand(COAST_B, 0);
}

int16_t MotorsX::getStatus() {
  Wire.requestFrom(address, 2);    // request 2 bytes from peripheral device #5
  // delay(1);
  uint8_t lo, hi;
  int16_t value = -99;
  if (Wire.available() == 2) {
    lo = Wire.read();    // receive low byte
    hi = Wire.read();    // receive high byte
    value = ((0x0000 | hi) << 8) | (0x0000 | lo);
  }
  else value = -9;  // error code
  return value;
}

// ------------------------------

Display::Display() {
  // initialise
}

void Display::start() {
  begin(&Adafruit128x64, 0x3c);
  displayRemap(true);
  invertDisplay(false);
  // setFont(fixed_bold10x15);  // bigger
  // setFont(font8x8);  // smaller
  setFont(X11fixed7x14B);
}

void Display::setRow(byte row) {
  setCursor(0, 2 * row - 2);
}

// ------------------------------

void ColorSensorA::start() {
  if (!init()) Serial.println("APDS error!");
  enableLightSensor(false);
}

void ColorSensorA::getRGB(uint16_t &_r, uint16_t &_g, uint16_t &_b) {
  getRGB();
  _r = r;
  _g = g;
  _b = b;
}

void ColorSensorA::getRGB() {
  readRedLight(r);
  readGreenLight(g);
  readBlueLight(b);
  r *= 5; g *= 4; b *= 3;  // white balance
  r = max(1, r-r0);
  g = max(1, g-g0);
  b = max(1, b-b0);
}

void ColorSensorA::calibrate() {
  uint16_t _r0, _g0, _b0;
  reset();
  getRGB(_r0, _g0, _b0);
  r0 = _r0; g0 = _g0; b0 = _b0;
}

void ColorSensorA::reset() {
  r0 = 0; g0 = 0; b0 = 0;
}

int16_t ColorSensorA::hue(uint16_t _r, uint16_t _g, uint16_t _b) {
  return round(57.3 * atan2(1.732*(_g-_b), 2*_r-_g-_b));
}

int16_t ColorSensorA::hue() {
  return hue(r, g, b);
}

int16_t ColorSensorA::color(uint16_t _r, uint16_t _g, uint16_t _b) {
  int16_t _hue = hue(_r,_g,_b);
  if (intens(_r,_g,_b) < 8) return BLACK;
  else if ((saturation(_r,_g,_b) < 35) && (intens(_r,_g,_b) > 30)) return WHITE;
  else if (_hue > -20 && _hue <= 15) return RED;
  else if (_hue > 15  && _hue <= 60) return YELLOW;
  else if (_hue > 60  && _hue <= 175) return GREEN;
  else if (_hue > 175 || _hue <= -20) return BLUE;
  else return BLACK;
}

int16_t ColorSensorA::color() {
  return color(r, g, b);
}

int16_t ColorSensorA::saturation(uint16_t _r, uint16_t _g, uint16_t _b) {
  float _sat;
  _sat = 1.0  - 1.0 * min(min(_r, _g), _b) / max(max(max(_r, _g), _b), 1);
  return round(100 * _sat);
}

int16_t ColorSensorA::saturation() {
  return saturation(r, g, b);
}

int16_t ColorSensorA::intens(uint16_t _r, uint16_t _g, uint16_t _b) {
  return (_r + _g + _b) / 3;
}

int16_t ColorSensorA::intens() {
  return intens(r, g, b);
}

// ------------------------------

void ColorSensorB::start() {
  // init(); does not function!
  setIntegrationTime(TCS34725_INTEGRATIONTIME_101MS);
  setGain(TCS34725_GAIN_4X);
}

void ColorSensorB::getRGB(uint16_t &_r, uint16_t &_g, uint16_t &_b) {
  getRGB();
  _r = r;
  _g = g;
  _b = b;
}

void ColorSensorB::getRGB() {
  uint16_t c;  // dummy
  getRawData(&r, &g, &b, &c);
  r *= 2; g *= 3; b *= 4;  // white balance
  r = max(1, r-r0);
  g = max(1, g-g0);
  b = max(1, b-b0);
}

void ColorSensorB::calibrate() {
  uint16_t _r0, _g0, _b0;
  reset();
  getRGB(_r0, _g0, _b0);
  r0 = _r0; g0 = _g0; b0 = _b0;
}

void ColorSensorB::reset() {
  r0 = 0; g0 = 0; b0 = 0;
}

int16_t ColorSensorB::hue(uint16_t _r, uint16_t _g, uint16_t _b) {
  return round(57.3 * atan2(1.732*(_g-_b), 2*_r-_g-_b));
}

int16_t ColorSensorB::hue() {
  return hue(r, g, b);
}

int16_t ColorSensorB::color(uint16_t _r, uint16_t _g, uint16_t _b) {
  int16_t _hue = hue(_r,_g,_b);
  if (intens(_r,_g,_b) < 400) return BLACK;
  else if ((saturation(_r,_g,_b) < 20) && (intens(_r,_g,_b) > 500)) return WHITE;
  else if (_hue > -100 && _hue <= 10) return RED;
  else if (_hue > 10  && _hue <= 90) return YELLOW;
  else if (_hue > -150  && _hue <= -100) return BLUE;
  else if (_hue > 90 || _hue <= -150) return GREEN;
  else return BLACK;
}

int16_t ColorSensorB::color() {
  return color(r, g, b);
}

int16_t ColorSensorB::saturation(uint16_t _r, uint16_t _g, uint16_t _b) {
  float _sat;
  _sat = 1.0  - 1.0 * min(min(_r, _g), _b) / max(max(max(_r, _g), _b), 1);
  return round(100 * _sat);
}

int16_t ColorSensorB::saturation() {
  return saturation(r, g, b);
}

int16_t ColorSensorB::intens(uint16_t _r, uint16_t _g, uint16_t _b) {
  return (_r + _g + _b) / 3;
}

int16_t ColorSensorB::intens() {
  return intens(r, g, b);
}