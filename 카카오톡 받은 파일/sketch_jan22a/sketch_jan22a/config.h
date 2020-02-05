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
#define modeSetLiquidVolume 3
#define modeSetLiquidInfo 4
#define modeSetActivePump 5
#define modeSetComplete 6
#define modeSetShowLiquidRateErrorMessage 100
#define ENABLE  true
#define DISABLE false

#define STOP  0
#define CW  1
#define CCW 2

#endif
