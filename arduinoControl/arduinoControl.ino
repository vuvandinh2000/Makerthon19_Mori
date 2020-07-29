#include <Servo.h>

#define MOTOR1 7
#define MOTOR2 6
#define MOTOR3 5
#define MOTOR4 4
#define MAX_SPEED 255

#define AIR_CONDITIONER 12
#define REAR_LIGHT 3
#define BUZZER 10
#define LIGHT 2
#define GREEN_LED 13

#define CAR_WINDOW 9

Servo window;
bool windowStatus = true;
bool Rear_Light = false;
void setup(){
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(MOTOR3, OUTPUT);
  pinMode(MOTOR4, OUTPUT);
  
  pinMode(AIR_CONDITIONER, OUTPUT);
  pinMode(REAR_LIGHT, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  window.attach(CAR_WINDOW);
  {
    Serial.begin(9600);
  } while (!Serial);
}

//speedMode = 1: max speed, 2: slow, 3: very slow, other: stop
void runMotor(int speedMode){
  switch (speedMode){
    case 1: digitalWrite(MOTOR1, LOW);
            analogWrite(MOTOR2, 255);
            analogWrite(MOTOR4, 255);
            digitalWrite(MOTOR3, LOW);
            //delay(10000);
    case 2: digitalWrite(MOTOR1, LOW);
            analogWrite(MOTOR2, 180);
            analogWrite(MOTOR4, 180);
            digitalWrite(MOTOR3, LOW);
    case 3: digitalWrite(MOTOR1, LOW);
            analogWrite(MOTOR2, 100);
            analogWrite(MOTOR4, 100);
            digitalWrite(MOTOR3, LOW);
    default: digitalWrite(MOTOR1, LOW);
             digitalWrite(MOTOR2, LOW);
             digitalWrite(MOTOR3, LOW);
             digitalWrite(MOTOR4, LOW);
  }
}

void airConditioner(bool b){
  if (b == true)
    digitalWrite(AIR_CONDITIONER, HIGH);
  else
    digitalWrite(AIR_CONDITIONER, LOW);
}

void rearLight(bool b){
  if (b == true)
    for (int i = 0; i <= 6; i++){
      delay(500);
      digitalWrite(REAR_LIGHT, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(500);
      digitalWrite(REAR_LIGHT, LOW);
      digitalWrite(BUZZER, LOW);
    }
  else
    digitalWrite(REAR_LIGHT, LOW);
    digitalWrite(BUZZER, LOW);
}

//1: blink, 0: off, other: on
void light(int n){
  switch (n){
    case 1: while (true){
              digitalWrite(LIGHT, HIGH);
              delay(400);
              digitalWrite(LIGHT, LOW);
              delay(400);
            }            
    case 0: digitalWrite(LIGHT, LOW);
    default:  digitalWrite(LIGHT, HIGH);
  }
}

void carWindow(bool b){
  if (b == true)
    for (int pos = 0; pos <= 90; pos++){
      window.write(pos);
      delay(10);
    }
   else
    for (int pos = 90; pos <= 180; pos++){
      window.write(pos);
      delay(10);
    }
}

void normal(){
  digitalWrite(MOTOR1, LOW);
  analogWrite(MOTOR2, 255);
  analogWrite(MOTOR4, 255);
  digitalWrite(MOTOR3, LOW);
  
  light(2);
  rearLight(false);
  
  airConditioner(true);
  if (windowStatus == false){
    carWindow(true);
    windowStatus = true;
  }
}

void level2(){
  if (Rear_Light == false) {
    rearLight(true);
    rearLight(false);
    Rear_Light = true;
  }
  
  digitalWrite(MOTOR1, LOW);
  analogWrite(MOTOR2, 200);
  analogWrite(MOTOR4, 200);
  digitalWrite(MOTOR3, LOW);
  
  airConditioner(false);
  if (windowStatus == true){
    carWindow(false);
    windowStatus = false;
  }
}

void level3(){
  rearLight(true);
  digitalWrite(MOTOR1, LOW);
  analogWrite(MOTOR2, 180);
  analogWrite(MOTOR4, 180);
  digitalWrite(MOTOR3, LOW);
  
  if (windowStatus = true){
    carWindow(false);
    windowStatus = false;
  }
  light(1);
}

void offAll(){
  rearLight(false);
  runMotor(5);
  light(0);
  if (windowStatus = true){
    carWindow(false);
    windowStatus = false;
  }
}

void loop() {
  if (Serial.available()){
    int state = Serial.parseInt();
    if (state == 2){
      level2();
      Serial.println("Level 2 Warning");
    }
    else if (state == 3){
      level3();
      Serial.println("Level 3 Warning");
    }
    else {
      normal();
      Serial.println("Normal");
    }
   }
}
