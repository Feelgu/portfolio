#include <SoftwareSerial.h>
#define RxD 2
#define TxD 3
SoftwareSerial blueToothSerial(RxD, TxD);//블루투스 rx tx 핀을 아두이노 2,3번핀에 사용
char recvChar;//수신받은 데이터를 저장할 변수

int buzzer = 6;//부저는 아두이노 6번핀에 사용
int vibe = 7;//진동센서는 아두이노 7번핀에 사용


void setup() {//전원이 들어와 아두이노아 구동이 되면 셋업이 1회 실행되고 loop()가 무한반복
  Serial.begin(9600);
  blueToothSerial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(vibe, OUTPUT);

  delay(1000);//1초 휴식
}

void loop() {//setup()이 1회 실행되고 무한 반복되는 부분

  if (blueToothSerial.available()) {//블루투스 데이터가 들어오면
    recvChar = blueToothSerial.read();//해당 변수에 수신받은 데이터를 저장한다.
    Serial.println(recvChar);//시리얼 모니터 수신받은 데이터 출력

    if (recvChar == 'a') {//a 데이터가 수신되면
      digitalWrite(vibe, HIGH);//진동센서 온
    }
    if (recvChar == 'b') {//b 데이터가 수신되면
      digitalWrite(vibe, LOW);//진동센서 오프
    }
    if (recvChar == 'c') {//c 데이터가 수신되면
      digitalWrite(buzzer, HIGH);//부저 온
    }
    if (recvChar == 'd') {//d 데이터가 수신되면
      digitalWrite(buzzer, LOW);//부저오프
    }

  }

  delay(100);
}
