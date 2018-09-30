#include <SoftwareSerial.h>
/*블루투스 자료 http://blog.naver.com/roboholic84/220594314436 */
//물건찾기 아두이노와 통신할 블루투스
#define RxD_2 4
#define TxD_2 5
SoftwareSerial blueToothSerial_2(RxD_2, TxD_2);//블루투스 rx tx 핀을 아두이노 4,5번핀에 사용
//스마트폰과 통신할 블루투스
#define RxD_1 2
#define TxD_1 3
SoftwareSerial blueToothSerial_1(RxD_1, TxD_1);//블루투스 rx tx 핀을 아두이노 2,3번핀에 사용
char recvChar;//수신받은 데이터를 저장할 변수



/*미세먼지 자료 http://blog.naver.com/roboholic84/220491144149 */
int measurePin = A0;//미세먼지센서 사용하기위한 변수 선언
int ledPower = A1;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

int dust = 0;//미세먼지 값을 저장할 변수
int count;



void setup() {//전원이 들어와 아두이노아 구동이 되면 셋업이 1회 실행되고 loop()가 무한반복
  Serial.begin(9600);

  blueToothSerial_2.begin(9600);
  pinMode(RxD_2, INPUT);
  pinMode(TxD_2, OUTPUT);

  blueToothSerial_1.begin(9600);
  pinMode(RxD_1, INPUT);
  pinMode(TxD_1, OUTPUT);

  pinMode(ledPower, OUTPUT);
  delay(1000);//1초 휴식
}

void loop() {//setup()이 1회 실행되고 무한 반복되는 부분
  //먼지센서값을 받아온다.
  dust = map(getdust(), 8, 133, 10, 100);
  count++;
  //count가 10이상일때
  if (count >= 10) {
    count = 0;//0으로 초기화 해주고

    Serial.print("dust: ");
    Serial.println(dust);//시리얼 모니터로 출력해준다.

    String s = "@" + (String)dust + "@#";

    int s_l = s.length();
    for (int i = 0; i < s_l; i ++) {
      blueToothSerial_1.print(s.charAt(i));//블루투스 통신으로 먼지센서값을 보낸다.
      delay(40);
    }

    //(EX 먼지센서값이 40이라면   @40@#   이라는 데이터를 보낸다. 안드로이드는 @이라는 데이터가 도착하면 데이터를 모으고 #이라는 데이터가 도착하면 데이터가 모두 도착한걸로 판단한다.)
  }

  if (blueToothSerial_1.available()) {//블루투스 데이터가 들어오면
    recvChar = blueToothSerial_1.read();//해당 변수에 수신받은 데이터를 저장한다.
    Serial.println(recvChar);//시리얼 모니터 수신받은 데이터 출력

    if (recvChar == 'a') {//스마트폰에서 a라는 데이터가 수신되면
      blueToothSerial_2.print("a");//먼지찾기 아두이노로 a라는 데이터를 그대로 보내준다.
    }
    if (recvChar == 'b') {
      blueToothSerial_2.print("b");
    }
    if (recvChar == 'c') {
      blueToothSerial_2.print("c");
    }
    if (recvChar == 'd') {
      blueToothSerial_2.print("d");
    }

  }
  delay(100);
}



int getdust() {
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (3.3 / 1024);
  dustDensity = 100 * calcVoltage - 0.1;
  return (dustDensity);
}
