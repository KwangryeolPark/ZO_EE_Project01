/*
 * 날짜 : 2020. 02. 03
 * 작성자 : 강수연, 김대원, 이학민
 * 목적 : 엔코더 프로그램과 LCD프로그램 pump프로그램 합치기
 *
 * 설명 :
 * attachInterrupt를 이용하여 기존의 엔코더 방식을 interrupt를 사용하여 구현했다.

 *
 */
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

int rate1;
int rate2;
int rate3;
int x = 0;
int cases = 0, count = 0;


void setup() {
  pinMode(7, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), calculateEncoderValue, CHANGE);
}

void loop() {



  if (digitalRead(7) == 0) {
    x++;
    delay(300);
  }
  switch (x % 7)
  {


    case 0:
      showMessage1();
      rate1 = count;
      break;

    case 1:
      x++;
      count = 0;
      break;

    case 2:
      showMessage2();
      rate2 = count;
      break;

    case 3:
      x++;
      count = 0;
      break;

    case 4:
      showMessage3();
      rate3 = count;
      break;

    case 5:
      x++;
      count = 0;
      break;
  }
}


void calculateEncoderValue(void) {
  int a, b;

  a = digitalRead(2);
  b = digitalRead(3);

  if (a == 1) {
    if (b == 0)  count++; //count++;
    else count--;//cases--;
  }

  count = constrain(count, 0, 9);
}
void showMessage1(void) {
  //lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("        1st");
  lcd.setCursor(0, 2);
  lcd.print("         " + String(rate1));
}

void showMessage2(void) {
  //lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("        2nd");
  lcd.setCursor(0, 2);
  lcd.print("         " + String(rate2));
}
void showMessage3(void) {
  //lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("        3rd");
  lcd.setCursor(0, 2);
  lcd.print("         " + String(rate3)      );
}
