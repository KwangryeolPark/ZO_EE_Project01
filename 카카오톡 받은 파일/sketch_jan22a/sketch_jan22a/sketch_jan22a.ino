#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

LiquidCrystal_I2C lcd(0x27, 20, 4); //or 0x3F
/*
 */
int64_t encoderCnt = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(checkButtonPin, INPUT_PULLUP);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(pumpPinA, OUTPUT);
  pinMode(pumpPinB, OUTPUT);
  pinMode(pumpPinC, OUTPUT);
  pinMode(mixPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderCnt, CHANGE);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, bar[0]);
  lcd.createChar(1, bar[1]);
  lcd.createChar(2, bar[2]);
  lcd.createChar(3, bar[3]);
  lcd.createChar(4, bar[4]);
  lcd.setCursor(0, 0);
  lcd.noCursor();
}

void loop() {
  static int16_t checkCnt = 0;
  static uint8_t liquidRate[3] = {0};
  static uint16_t totalLiquidVolume = 0;
  static uint16_t liquidVolume[3] = {0};
  static boolean checkActive = ENABLE;
  static uint16_t mode = modeSetLiquidRate1;

  checkButtonCnt(&checkCnt, checkActive);

  if (checkCnt % 5);

  switch (mode) {
    case modeSetLiquidRate1:
      showFirstMessage(liquidRate);
      break;

    case modeSetLiquidVolume:
      uint8_t totalLiquidRate = liquidRate[0] + liquidRate[1] + liquidRate[2];
      if (totalLiquidRate == 0) {
        mode = modeSetShowLiquidRateErrorMessage;
        break;
      }
      liquidVolume[0] = (totalLiquidVolume / totalLiquidRate) * liquidRate[0];
      liquidVolume[1] = (totalLiquidVolume / totalLiquidRate) * liquidRate[1];
      liquidVolume[2] = (totalLiquidVolume / totalLiquidRate) * liquidRate[2];

      break;

    case modeSetShowLiquidRateErrorMessage:
      showErrorMessage("Total Liquid Rate is 0!!");
      delay(2000);
      mode = modeSetLiquidRate1;
      checkCnt = 0;
      break;

    case modeSetLiquidInfo:

      ///Yes or No 선택지를 만들어서 Yes를 선택하면 modeSetActivePump모드로
      ///No를 선택하면 modeSetLiquidRate1모드로 돌아가기
      break;

    case modeSetActivePump:
      uint64_t presentTime = millis();
      uint64_t timeGap;
      uint16_t timeByVolume;
      for (int i = 0; i < 3; i++) {
        if(i == 0)  timeByVolume = liquidVolume[i] * mLToMilliSecondsA;
        else if(i == 1)  timeByVolume = liquidVolume[i] * mLToMilliSecondsB;
        else  timeByVolume = liquidVolume[i] * mLToMilliSecondsC;
        do {
          timeGap = millis() - presentTime;
          if(i == 0)  showPumpAProcess(uint16_t(float(timeGap / timeByVolume) * 100.0));
          else if(i == 1) showPumpBProcess(uint16_t(float(timeGap / timeByVolume) * 100.0));
          else  showPumpCProcess(uint16_t(float(timeGap / timeByVolume) * 100.0));
        } while (timeGap <= timeByVolume + 10);
      }
      ////////다음 모드로 넘어 가는 구문 적기
  }
}


















void checkButtonCnt(int *data, boolean flag) {
  if (flag == ENABLE) {
    if (checkButtonPressed) {
      delay(checkButtonDelay);
      while (checkButtonPressed);
      delay(checkButtonDelay);
      *data++;
    }
  }
}

void showFirstMessage(uint8_t *rate) {

}

void encoderCounting(void) {
  if (encoderStatusA) {
    if (!encoderStatusA) encoderCnt++;
    else encoderCnt--;
  } else {
    if (encoderStatusB) encoderCnt++;
    else encoderCnt--;
  }
}

void showErrorMessage(String msg) {

}

void showPumpAProcess(uint16_t Time) {

}
void showPumpBProcess(uint16_t Time) {

}
void showPumpCProcess(uint16_t Time) {

}
