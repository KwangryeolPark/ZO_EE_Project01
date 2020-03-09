#ifndef _MQ_GAS_H
#define _MQ_GAS_H


#ifdef ARDUINO
  #if (ARDUINO >= 100)
  #include <Arduino.h>
  #else
  #include <WProgram.h>
  #include <pins_arduino.h>
  #endif
#endif

#ifdef TARGET_LPC1768
  #include <Arduino.h>
#endif

#define CAL_10_Times  1
#define CAL_100_Times 2
#define MQ_3  3
#define Alcohol 0
#define RH33  0
#define RH85  1

class MQ_GAS {
public:
  MQ_GAS(uint8_t);
  void init();
  void setAnalogPin(uint8_t);
  void calibrate(uint8_t);
  void setR2(uint16_t);
  void setGraphPoints(float *, float *);
  void setGas(uint8_t);
  float density(void);

private:
  uint8_t _product = 0;
  uint8_t _gas = Alcohol;
  uint8_t _analogPin = 0;
  uint16_t _R0 = 0;
  uint16_t _R0Analog = 0;
  uint16_t _R2 = 10.0e2;
  uint16_t _RS = 0;
  uint8_t _RH = RH33;
  float _Fixed_Rs_R0[10];
  float _Fixed_mg_L[10];
  void convertAnalogToVoltage(uint16_t analog, float *voltage);
  void calculateSensorResistance(float voltage, uint16_t *resistance);
  void convertAnalogToResistance(uint16_t analog, uint16_t *resistance);
};
#endif
