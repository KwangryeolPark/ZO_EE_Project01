
/*
 * 날짜 : 2020. 01. 27
 * 작성자 : 강수연, 김대원, 이학민
 * 목적 : 엔코더(Encoder) 다루기, 시리얼 모니터 다루기
 * 
 * 설명 : 
 * 엔코더의 A펄스 핀을 D2,
 * 엔코더의 B펄스 핀을 D3에 연결한다.
 * 이를 INPUT모드로 동작시키고 Serial통신 속도를 9600BAUD로 설정한다.(Serial.begin(9600))
 * 
 * 엔코더의 펄스 값을 카운팅 할 변수 cases를전역 변수로 초기화한다.
 * 또한 엔코더 펄스 값을 저장할 두 변수 a, b와 엔코더 펄스의 변화를 감기하기 위해서 만든 전 상태 값(a1, b1)을 정적 변수로 초기화한다.
 * 이때 a1, b1에 static을 사용한 이유는 처음 초기화 될 때는 원래의 a값과 b값이 들어가야하기 때문이다.
 * 
 * a > a1조건문을 통해 펄스가 LOW -> HIGH로 변했는지를 판단한다.
 * 
 * 엔코더를 한 번 돌릴 때마다 2개의 펄스가 생성되는 것을 확인했고, 
 * 그에따라 cases가 2또는 -2일 때 실질적인 돌려진 값을 저장하기 위해 count값을 만들었다.
 * 
 */


void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);    //D2를 INPUT모드로
  pinMode(3, INPUT);    //D2를 INPUT모드로

  Serial.begin(9600);   //Serial 통신 속도를 9600BAUD로
}

int cases = 0, count = 0;
int a, b;

void loop() {
  a = digitalRead(2); //D2의 값을 a에 저장
  b = digitalRead(3); //D3의 값을 b에 저장
  static int a1 = a;  //처음 초기화 될 때 a값을 저장하기 위해 static으로 선언
  static int b1 = b;  //처음 초기화 될 때 b값을 저장하기 위해 static으로 선언

  if (a > a1) {     //LOW -> HIGH를 판단
    if (b == 1) { 
      cases--;
    }
    else {
      cases++;
    }
  }
  else if (a < a1) {  //HIGH -> LOW를 판단
    if (b == 0) {
      cases--;
    }
    else {
      cases++;
    }
  }
  
  if (cases == 2) { //cases가 2인 경우
    count++;      //count값을 증가
    cases = 0;    //cases는 0으로
  }
  else if(cases == -2) { 
    count--;
    cases = 0;
    if (count < 0) count = 0;
  }

  Serial.println(String(count) + "\t" + String(cases)); //변수들의 값을 확인하기 위해 Serial모니터를 사용
  //  delay(100);
  a1 = a;//digitalRead(2);  //a1값에 a값을 대입하여, 코드상으로 36번째 줄이 실행 될 때 a1이 a의 과거 값임을 지정
  b1 = b;//digitalRead(3);  //b1값에 b값을 대입하여, 코드상으로 36번째 줄이 실행 될 때 b1이 b의 과거 값임을 지정
}
