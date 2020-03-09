#include "MQ_GAS.h"
#include "MQ_GRAPH_POINTS.h"

MQ_GAS::MQ_GAS(uint8_t product) {
  _product = product;
};
void MQ_GAS::init() {
  pinMode(_analogPin, INPUT);
  switch(_product) {
    case MQ_3:
    switch(_gas) {
      case Alcohol:
      setGraphPoints(_MQ3_Fixed_Rs_R0, _MQ3_Fixed_mg_L);
      break;
    }
    break;
  }
}
void MQ_GAS::setAnalogPin(uint8_t analogPin) {
  _analogPin = analogPin;
};

void MQ_GAS::calibrate(uint8_t cntMode) {
  _R0Analog = 0;
  switch(cntMode) {
    case CAL_10_Times:
      for(int i = 0; i < 10; i++) _R0Analog += analogRead(_analogPin);
      _R0Analog /= 10;
      break;

    case CAL_100_Times:
      for(int ii = 0; ii < 10; ii++) {
        uint16_t temp = 0;
        for(int i = 0; i < 10; i++) {
          temp += analogRead(_analogPin);
        }
        temp /= 10;
        _R0Analog += temp;
      }
      _R0Analog /= 10;
      break;
  }
};
void MQ_GAS::setGas(uint8_t gas) {
  _gas = gas;
}
void MQ_GAS::convertAnalogToVoltage(uint16_t analog, float *voltage) {
  *voltage = float(analog) / 1023.0f * 5.0f;
};
void MQ_GAS::calculateSensorResistance(float voltage, uint16_t *resistance) {
  *resistance = (5.0 * _R2 / voltage) - _R2;
};
void MQ_GAS::convertAnalogToResistance(uint16_t analog, uint16_t *resistance) {
  float temp = 0;
  convertAnalogToVoltage(analog, &temp);
  calculateSensorResistance(temp, resistance);
};

void MQ_GAS::setR2(uint16_t resistance) {
  _R2 = resistance;
};

void MQ_GAS::setGraphPoints(float *RS_R0, float *mgL) {
  for(int i = 0; i < 10; i++) {
    _Fixed_Rs_R0[i] = RS_R0[i];
    _Fixed_mg_L[i] = mgL[i];
  }
}

float MQ_GAS::density(void) {
  uint16_t value = analogRead(_analogPin);

  convertAnalogToResistance(value, &_RS);
  convertAnalogToResistance(_R0Analog, &_R0);

  float x = float(_RS) / float(_R0);

  for(int i = 9; i > 0; i--) {
    if(_Fixed_Rs_R0[i] <= x && x < _Fixed_Rs_R0[i - 1]) {
      float tangent = (_Fixed_mg_L[i - 1] - _Fixed_mg_L[i]) / (_Fixed_Rs_R0[i - 1] - _Fixed_Rs_R0[i]);

      return tangent * (x - _Fixed_Rs_R0[i]) + _Fixed_mg_L[i];
    }
  }
};
