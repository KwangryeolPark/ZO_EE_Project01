#ifndef _CONFIG_H
#define _CONFIG_H


#define checkButtonPin  4
#define checkButtonPressed  !digitalRead(checkButtonPin)
#define checkButtonReleased digitalRead(checkButtonPin)
#define checkButtonDelay    100


#define encoderPinA 2
#define encoderPinB 3

#define encoderStatusA  !digitalRead(encoderPinA)
#define encoderStatusB  !digitalRead(encoderPinB)


#define pumpPinA  9
#define pumpPinB  10
#define pumpPinC  11
#define mixPin    5

#define controlPumpA(flag)  digitalWrite(pumpPinA, flag)
#define controlPumpB(flag)  digitalWrite(pumpPinB, flag)
#define controlPumpC(flag)  digitalWrite(pumpPinC, flag)
#define controlMixer(flag)  digitalWrite(mixPin, flag)

#define mLToMilliSecondsA 50
#define mLToMilliSecondsB 50
#define mLToMilliSecondsC 50


#define modeSetLiquidRate1  0
#define modeSetLiquidRate2  1
#define modeSetLiquidRate3  2
#define modeSetLiquidInfo 3
#define modeSetActivePump 4
#define modeSetLiquidVolume 5
#define modeSetShowLiquidRateErrorMessage 6
#define ENABLE  true

byte bar[5][8] = {
  { B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000
  },
  { B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000
  },
  { B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100
  },
  { B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110
  },
  { B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  }
};

#endif
