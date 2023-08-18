//RobotNest SumoBoy 2.0 with dispaly, test code
//For questions write to: karlis.berkolds@gmail.com

//To use display, please download adafruit libraries from:https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples
//Necessary libraries for display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {//Setup
  // display setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //LED definitions
  pinMode(12, OUTPUT); //Left LED
#define led1ON   digitalWrite(12,HIGH);
#define led1OFF  digitalWrite(12,LOW);
  pinMode(6, OUTPUT); //Right LED
#define led2ON   digitalWrite(6,HIGH);
#define led2OFF  digitalWrite(6,LOW);
  pinMode(13, OUTPUT); //Back LED
#define led3ON   digitalWrite(13,HIGH);
#define led3OFF  digitalWrite(13,LOW);
  pinMode(17, OUTPUT); //Orange_LED
#define led4ON   digitalWrite(17,HIGH);
#define led4OFF  digitalWrite(17,LOW);
  //Button definitions
  pinMode(A2, INPUT);
  digitalWrite(A2, HIGH);
#define button1 !digitalRead(A2)
  //Turn on serial port monitor
  Serial.begin(9600);
  //DIP switch Definition
  pinMode(7, INPUT);
  digitalWrite(7, HIGH);
#define DIP1 !digitalRead(7)
  pinMode(15, INPUT);
  digitalWrite(15, HIGH);
#define DIP2 !digitalRead(15)
  pinMode(16, INPUT);
  digitalWrite(16, HIGH);
#define DIP3 !digitalRead(16)
  pinMode(14, INPUT);
  digitalWrite(14, HIGH);
#define DIP4 !digitalRead(14)
  //Line sensor definitions
  pinMode(A4, INPUT);
#define senLeft  analogRead(A4)<32
  pinMode(A3, INPUT);
#define senRight analogRead(A3)<31
  pinMode(A5, INPUT);
#define senBack  analogRead(A5)<31
  //Proximity sensor definition
  pinMode(A0, INPUT); //PROX1 middle left sensor
#define PROX1 !digitalRead(A0)
  pinMode(A1, INPUT); //PROX2 middle senor
#define PROX2 !digitalRead(A1)
  pinMode(4, INPUT); //PROX3 middle right sensor
#define PROX3 !digitalRead(4)

  //  Motor definition
  //MOTOR LEFT
  pinMode(5, OUTPUT);
  pinMode(11, OUTPUT);
  //MOTOR RIGHT
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
//IR receiver, if attached
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  #define Start digitalRead(0)
  #define Stop digitalRead(1)
}
//Motor control functions
void Forward (int leftSpeed, int rightSpeed) {
  analogWrite(5, leftSpeed);
  digitalWrite(11, LOW);
  analogWrite(9, rightSpeed);
  digitalWrite(10, LOW);
}
void Backward (int leftSpeed, int rightSpeed) {
  analogWrite(11, leftSpeed);
  digitalWrite(5, LOW);
  analogWrite(10, rightSpeed);
  digitalWrite(9, LOW);
}
void Left (int leftSpeed, int rightSpeed) {
  analogWrite(11, leftSpeed);
  digitalWrite(5, LOW);
  analogWrite(9, rightSpeed);
  digitalWrite(10, LOW);
}
void Right (int leftSpeed, int rightSpeed) {
  analogWrite(5, leftSpeed);
  digitalWrite(11, LOW);
  analogWrite(10, rightSpeed);
  digitalWrite(9, LOW);
}

int senState = 0; //Variable for saving current sensor State
void loop() { //infinite loop

  // Show sensor values on display
  display.setCursor(0, 0);
  display.clearDisplay();
  display.print("SenLeft: "); display.println(analogRead(A4));
  display.print("SenRight: "); display.println(analogRead(A3));
  display.print("SenBack: "); display.println(analogRead(A5));
  display.print("S1: "); display.print(PROX1);display.print(" ");
  display.print("S2: "); display.print(PROX2);display.print(" ");
  display.print("S3: "); display.println(PROX3);
  display.display();
  delay(100);
  display.clearDisplay();

  //Push Dip switch to test motors
  while (DIP1) {
    Forward(100, 100);
  }
  while (DIP2) {
    Backward(100, 100);
  }
  while (DIP3) {
    Left(100, 100);
  }
  while (DIP4) {
    Right(100, 100);
  }
  
  if (senLeft) {
    led1ON; 
  } else {
    led1OFF;
  }
  if (senRight) {
    led2ON;
  } else {
    led2OFF;
  }
  if (senBack) {
    led3ON;
  } else {
    led3OFF;
  }
  Forward(0, 0); //Stop motors

  //Show senor values on serial monitor
  Serial.print("SenLeft: "); Serial.println(analogRead(A4));
  Serial.print("SenRight: "); Serial.println(analogRead(A3));
  Serial.print("SenBack: "); Serial.println(analogRead(A5));
  Serial.print("P1: "); Serial.println(PROX1);
  Serial.print("P2: "); Serial.println(PROX2);
  Serial.print("P3: "); Serial.println(PROX3);


  if (button1)//push button1 to start fight code
  {
    while (button1) { //loop while button is pushed
    }
    led1ON;
    delay(1000);
    led2ON;
    delay(1000);
    led3ON;
    delay(1000);
    led1OFF;
    delay(1000);
    led2OFF;
    delay(1000);
    led3OFF;

    while (!button1) // robot stops if button 1 is pressed again
    {
      senState = 0; 
      if (senLeft) {
        led1ON;
        senState = senState + 1;
      } else {
        led1OFF; //add to senState binary sensor value
      }
      if (senRight) {
        led2ON;
        senState = senState + 2;
      } else {
        led2OFF;
      }
      if (senBack) {
        led3ON;
        senState = senState + 4;
      } else {
        led3OFF;
      }


      if (senState == 0) { //PROXIMITY sensor is only counted if robot is on black field
        if (PROX1) {
          senState = +8;
        }
        if (PROX2) {
          senState += 16;
        }
        if (PROX3) {
          senState += 32;
        }
      }

      display.setCursor(0, 0);
      display.clearDisplay();
      display.print("SenLeft: "); display.println(senLeft);
      display.print("SenRight: "); display.println(senRight);
      display.print("SenBack: "); display.println(senBack);
      display.print("SenState: "); display.println(senState);
      display.display();
      display.clearDisplay();

      switch (senState) { //Switch structure reads variable and determines motor action
        case 0://robot is on black field and there is no opponent
          Forward(40, 50);
          break;
        case 1: //Left line sensor
          Backward(150, 150);
          delay(200);
          Right(50, 50);
          delay(100);
          break;
        case 2://right line sensor
          Backward(150, 150);
          delay(200);
          Left(50, 50);
          delay(100);
          break;
        case 3://front line sensor
          Backward(150, 150);
          delay(100);
          break;
        case 4://back line sensor
          Forward(100, 100);
          delay(200);
          break;
        case 5://back and left line sensor
          Right (100, 100);
          delay(100);
          Forward(50, 50);
          delay(100);
          break;
        case 6://back and right line sensor
          Left (100, 100);
          delay(100);
          Forward(50, 50);
          delay(100);
          break;
        case 7://all line sensors
          Forward(0, 0);
          break;

        // Motor commands if proximity sensors has detected opponent
        case 8://PROX1
          Right(100, 100);
          break;
        case 16://PROX2
          Forward(100, 100);
          break;
        case 24://PROX1 & PROX2
          Forward(100, 50);
          break;
        case 32://PROX3
          Left(100, 100);
          break;
        case 40://PROX1 &PROX3
          Forward(100, 100);
          break;
        case 48://PROX2 &PROX3
          Forward(50, 100);
          break;
        case 56://PROX1 &PROX2& PROX3
          Forward(150, 150);
          break;


      }
    }
    while (button1) { //loop for stability
    }
  }
}













