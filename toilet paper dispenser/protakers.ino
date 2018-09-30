#include <Servo.h>

//button
const int buttonPin = 2;

//led 6ea
const int ledPin0 = 15;
const int ledPin1 = 16;
const int ledPin2 = 14;
const int ledPin3 = 17;
const int ledPin4 = 18;
const int ledPin5 = 19;

const int LED[]={ledPin0, ledPin1, ledPin2, ledPin3, ledPin4, ledPin5};

//L298N motor
const int L298N_IN1 = 9;
const int L298N_IN2 = 10;
const int en = 11;

//servo motor
Servo servomotor;
int pos=0;

// over 6th led
bool over;

//led on/off count
int count;

void setup() {

    over = false;

    //button
    pinMode(buttonPin,INPUT);

    //led
    pinMode(ledPin0,OUTPUT);
    pinMode(ledPin1,OUTPUT);
    pinMode(ledPin2,OUTPUT);
    pinMode(ledPin3,OUTPUT);
    pinMode(ledPin4,OUTPUT);
    pinMode(ledPin5,OUTPUT);

    //L298N motor
    pinMode(L298N_IN1, OUTPUT);
    pinMode(L298N_IN2, OUTPUT);
    pinMode(en, OUTPUT);

    //servo motor
    servomotor.attach(12);

}

void loop() {
   
  count=0;

  // button을 누른 경우
  while(!digitalRead(buttonPin)){

    // 누르는 시간에 따라 위로 불빛이 하나씩 증가함
    analogWrite(LED[count++], 200);

    //motor작동
    digitalWrite(L298N_IN1, LOW);
    digitalWrite(L298N_IN2, HIGH);
    analogWrite(en, 100);
    //30으로
    digitalWrite(L298N_IN1, LOW);
    digitalWrite(L298N_IN2, HIGH);
    analogWrite(en, 30);
    delay(700);

    // 6개의 불빛이 켜진 경우
    if(count==6){
           over = true;
           break;
       }
   }

   // 버튼을 1회 이상 누른 경우
   if(count != 0){
     //motor stop
    digitalWrite(L298N_IN1, LOW);
    digitalWrite(L298N_IN2, LOW);
    analogWrite(en, 0);
    delay(200);
    
    //잡아주고
    for(pos=0;pos<150;pos++){
      servomotor.write(pos);
      delay(10); 
    }
   
    //끊어주고
    digitalWrite(L298N_IN1, HIGH);
    digitalWrite(L298N_IN2, LOW);
    analogWrite(en, 255);
    delay(250);
    digitalWrite(L298N_IN1, LOW);
    digitalWrite(L298N_IN2, LOW);
    analogWrite(en, 0);
 
     //led 끄기
     while(count>-1) {
       analogWrite(LED[count--],0);
       delay(150);
     }
     
    for(pos=150;pos>0;pos--){
      servomotor.write(pos);
      delay(10); 
    }
   }
   // 6개의 led가 켜진 경우
   if(over){
     delay(5000);
     over = false;
   }
}
