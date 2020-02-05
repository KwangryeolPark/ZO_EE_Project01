#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

LiquidCrystal_I2C lcd(0x27, 20, 4); //or 0x3F
/*
 */
int64_t encoderCnt = 0; //엔코더 펄스 값이 저장되어있음
int64_t pEncoderCnt = 0;//이전 엔코더 펄스 값이 저장되어있음
uint8_t encoderRotateDirection = STOP;  //엔코더의 회전 방향 값이 저장되어있음

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

  checkButtonCnt(&checkCnt, checkActive); //버튼 눌린 횟수는 checkCnt에 저장, checkActive는 버튼 기능 유무

  if(checkActive == ENABLE) {
    switch (checkCnt % 7) {
      case 0:
        mode = modeSetLiquidRate1:
        break;

      case 1:
        mode = modeSetLiquidRate2:
        break;

      case 2:
        mode = modeSetLiquidRate3:
        break;

      case 3:
        mode = modeSetLiquidVolume;
        break;

      case 4:
        mode = modeSetLiquidInfo;
        break;

      case 5:
        mode = modeSetActivePump;
        break;

      case 6:
        mode = modeSetComplete;
        break;
    }
  }


  if(encoderCnt > pEncoderCnt) encoderRotateDirection = CW;
  else if(encoderCnt < pEncoderCnt) encoderRotateDirection = CCW;
  else encoderRotateDirection = STOP;



  switch (mode) {
    case modeSetLiquidRate1:
      checkActive = ENABLE;
      if(encoderRotateDirection == CW) liquidRate[0]++;
      else if(encoderRotateDirection == CCW) liquidRate[0]--;
      liquidRate[0] = constrain(liquidRate[0], 0, 10);
      showRateMessage(liquidRate, 0);
      break;

      case modeSetLiquidRate2:
        checkActive = ENABLE;
        if(encoderRotateDirection == CW) liquidRate[1]++;
        else if(encoderRotateDirection == CCW) liquidRate[1]--;
        liquidRate[1] = constrain(liquidRate[1], 0, 10);
        showRateMessage(liquidRate, 1);
        break;

        case modeSetLiquidRate3:
          checkActive = ENABLE;
          if(encoderRotateDirection == CW) liquidRate[2]++;
          else if(encoderRotateDirection == CCW) liquidRate[2]--;
          liquidRate[2] = constrain(liquidRate[2], 0, 10);
          showRateMessage(liquidRate, 2);
          break;


    case modeSetLiquidVolume:
      checkActive = ENABLE;
      uint8_t totalLiquidRate = liquidRate[0] + liquidRate[1] + liquidRate[2];
      if (totalLiquidRate == 0) {
        mode = modeSetShowLiquidRateErrorMessage;
        checkActive = DISABLE;
        break;
      }
      liquidVolume[0] = (totalLiquidVolume / totalLiquidRate) * liquidRate[0];
      liquidVolume[1] = (totalLiquidVolume / totalLiquidRate) * liquidRate[1];
      liquidVolume[2] = (totalLiquidVolume / totalLiquidRate) * liquidRate[2];
      mode = modeSetLiquidInfo;
      checkCnt++;

      break;



    case modeSetLiquidInfo:
      checkActive = DISABLE;
      static int select = 0;
      int tempCnt = 0;
      showLiquidMessage(liquidRate, select);
      if(encoderRotateDirection == CW) {
        select = 0;
        checkButtonCnt(&tempCnt, ENABLE);
        if(tempCnt != 0) {
//          checkActive = ENABLE;
          mode = modeSetLiquidRate1;
          checkCnt = 0;
          break;
        }
      } else if(encoderRotateDirection == CCW) {
        select = 1;
        checkButtonCnt(&tempCnt, ENABLE);
        if(tempCnt != 0) {
          mode = modeSetActivePump;
          checkCnt = 0;
          break;
        }
      }
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
          showPumpsProcess(i, uint16_t(float(timeGap / timeByVolume) * 100.0));
/*          if(i == 0)  showPumpAProcess(uint16_t(float(timeGap / timeByVolume) * 100.0));
          else if(i == 1) showPumpBProcess(uint16_t(float(timeGap / timeByVolume) * 100.0));
          else  showPumpCProcess(uint16_t(float(timeGap / timeByVolume) * 100.0));*/
        } while (timeGap <= timeByVolume + 10);
      }
      delay(3000);
      activeBuzzer(2000);
      checkCnt++;
      mode = modeSetComplete;
      break;
      ////////다음 모드로 넘어 가는 구문 적기

      case modeSetComplete:
        showCompMessage("Remove the bottle\nPress the button");
        checkActive = ENABLE;
        delay(300);
      break;






      case modeSetShowLiquidRateErrorMessage:
        showErrorMessage("Total Liquid Rate is 0!!");
        delay(2000);
        mode = modeSetLiquidRate1;
        checkCnt = 0;
        break;

  }

  pEncoderCnt = encoderCnt;



  static uint16_t pMode = mode;
  if(mode != pMode) lcd.clear();

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

void showRateMessage(uint8_t *rate, uint8_t select) {
  lcd.setCursor(0, 0);
  lcd.print("    Liquids Rate    ");

  for(int j = 0; j < 3; j++) {
    lcd.setCursor(j + 1, 0);
    lcd.print(((select == j) ? "*":" ") + String(j + 1) + " : ");
    lcd.setCursor(j + 1, 4);
    for(int i = 0; i < 10; i++) {
      if(i < rate[j])  lcd.write(0xFF);
      else lcd.write(0x00);
    }
    lcd.setCursor(j + 1, 17);
    lcd.print(String(rate[j]) + String("  "));
  }

}

void encoderCounting(void) {
  if (encoderStatusA) {
    if (!encoderStatusA) encoderCnt++;
    else encoderCnt--;
  }/* else {
    if (encoderStatusB) encoderCnt++;
    else encoderCnt--;
  }*/

}
/*
void showErrorMessage(String msg) {
  lcd.setCursor(0, 0);
  lcd.print(msg);
}

void showPumpAProcess(uint16_t Time) {

}
void showPumpBProcess(uint16_t Time) {

}
void showPumpCProcess(uint16_t Time) {

}*/

void showPumpsProcess(int i, uint16_t percent) {
  lcd.setCursor(0, 0);
  lcd.print("    Process");
  lcd.setCursor(i + 1, 0);
  lcd.print("*" + String(i + 1) + ": ");

  uint16_t p100 = percent;
  p100 = percent;
  for(int i = 0; i < int(map(p100, 0, 100, 0, 10)); i++) lcd.write(0xFF);
  lcd.print(" " + String(percent) + "  ");
}

void showLiquidMessage(uint8_t *rate, int select) {
  lcd.setCursor(0, 0);
  lcd.print("    1 :  2  :  3");
  lcd.setCursor(1, 0);
  lcd.print("   " + ((rate[0] != 10) ? String(rate[0]) : " ") + " : " + ((rate[1] != 10) ? String(rate[1]) : " ") + " : " + ((rate[2] != 10) ? String(rate[2]) : " ") + "    ");
  lcd.setCursor(2, 0);
  lcd.print("       300mL         ");
  lcd.setCursor(3, 0);
  lcd.print("   Okay     Back    ");
  if(select == 0) { //Back
    lcd.setCursor(3, 11);
    lcd.print("*");
  }
  else {
    lcd.setCursor(3, 2);
    lcd.print(" ");
  }
}


void showCompMessage(String msg) {
  lcd.setCursor(0, 0);
  lcd.print(msg);
}

void activeBuzzer(int de) {

}
