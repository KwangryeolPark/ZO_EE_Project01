#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

/*
 * 날짜 : 2020. 01. 20
 * 작성자 : 강수연, 김대원, 이학민
 * 목적 : 버튼 다루기
 *
 * 설명 :
 * 아두이노 D7 버튼에 PULLUP저항 회로의 형태로 버튼을 설치한다.
 * 버튼이 눌리면 0, 버튼이 안 눌리면 1을 반환하는 digitalRead(7)를 이용하여
 * 버튼이 눌린 횟수를 카운팅한다.(x++)
 * 버튼의 연속적인 눌림을 방지하기 위해 delay(300)을 추가한다.
 *
 * lcd를통해 미리 저장된 rate값들을 화면에 표시하는 간단한 출력을 한다.
 * x % 3연산을 통해 화면 전환 기능을 구현한다.
 */

int rate1;
int rate2;
int rate3;
int x = 0;  //버튼이 눌린 횟수

void setup() {
  pinMode(7, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop() {


  rate1 = 5;
  rate2 = 3;
  rate3 = 6;

  if (digitalRead(7) == 0) {
    x++;
    delay(300);
  }

  switch (x % 3)
  {
    case 0:
      showMessage1();
      break;

    case 1:
      showMessage2();
      break;

    case 2:
      showMessage3();
      break;
  }
}

void showMessage1(void) {
  // lcd.clear();
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
