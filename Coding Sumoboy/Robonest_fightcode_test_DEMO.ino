void setup() {//uzstādīšana     
  //LED definīcijas
  pinMode(13, OUTPUT);//Arduino LED 
#define ledAON   digitalWrite(13,HIGH)
#define ledAOFF  digitalWrite(13,LOW)
  pinMode(5,OUTPUT);//Left LED
#define led1ON   digitalWrite(5,HIGH);
#define led1OFF  digitalWrite(5,LOW);
  pinMode(11,OUTPUT);//Right LED
#define led2ON   digitalWrite(11,HIGH);
#define led2OFF  digitalWrite(11,LOW);
  pinMode(2,OUTPUT);//Back LED
#define led3ON   digitalWrite(2,HIGH);
#define led3OFF  digitalWrite(2,LOW);
  //Pogu definicijas
  pinMode(12,INPUT);
  digitalWrite(12,HIGH);
#define button1 !digitalRead(12)  
  //Seriālā porta monitora ieslēgšana
  Serial.begin(9600);
  //DIP Slēdžu definīcija 
  pinMode(15,INPUT);
  digitalWrite(15,HIGH);
#define DIP1 digitalRead(15) 
  pinMode(14,INPUT);
  digitalWrite(14,HIGH);
#define DIP2 digitalRead(14)
  pinMode(16,INPUT);
  digitalWrite(16,HIGH);
#define DIP3 digitalRead(16) 
  pinMode(17,INPUT);
  digitalWrite(17,HIGH);
#define DIP4 digitalRead(17)
  //Līnijas sensoru definīcija
  pinMode(A2,INPUT);
#define senLeft  analogRead(A2)<75
  pinMode(A1,INPUT);
#define senRight analogRead(A1)<75
  pinMode(A4,INPUT);
#define senBack  analogRead(A4)<75
  //SHARP sensoru definīcija
  pinMode(A5,INPUT);
#define SHARP1 !digitalRead(A5) 
  pinMode(A3,INPUT);//SHARP2
#define SHARP2 !digitalRead(A3)
  pinMode(7,INPUT);//SHARP3
#define SHARP3 !digitalRead(7)
  pinMode(A8,INPUT);//SHARP4
#define SHARP4 !digitalRead(A8)
  pinMode(A7,INPUT);//SHARP5
#define SHARP5 !digitalRead(A7)
  //Motoru definīcija
  pinMode(3,OUTPUT);//kreisais PWM
  pinMode(4,OUTPUT);//kreisais Direction
  pinMode(9,OUTPUT);//labais PWM
  pinMode(10,OUTPUT);//labais Direction
}
//Motoru vadības funkciju definīcijas
void Forward (int leftSpeed, int rightSpeed){
  analogWrite(3,leftSpeed);
  digitalWrite(4,LOW); 
  analogWrite(9,rightSpeed); 
  digitalWrite(10,LOW);  
} 
void Backward (int leftSpeed, int rightSpeed){
  analogWrite(3,leftSpeed);
  digitalWrite(4,HIGH);
  analogWrite(9,rightSpeed);
  digitalWrite(10,HIGH);
}
void Left (int leftSpeed, int rightSpeed){
  analogWrite(3,leftSpeed);
  digitalWrite(4,HIGH);
  analogWrite(9,rightSpeed);
  digitalWrite(10,LOW);
}
void Right (int leftSpeed, int rightSpeed){
  analogWrite(3,leftSpeed);
  digitalWrite(4,LOW);
  analogWrite(9,rightSpeed);
  digitalWrite(10,HIGH);
}

int motState =0;//Izveidojam mainīgo, kur norādīsim motora darbību
void loop() { //mūžīgais cikls
  Forward(0,0);//Izsaucam funkciju motoru apstādināšanai
 // Serial.print("SenLeft: ");Serial.println(analogRead(A2));
 // Serial.print("RightLeft: ");Serial.println(analogRead(A1));
 // Serial.print("BackLeft: ");Serial.println(analogRead(A4));
  delay(500);
  if (button1)//robots sāk darboties ar Button1 nospiešanu
  {
    while (button1){ //Cikls, kamēr poga ir nospiesta
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
    while (!button1) // robots apstājas līdz ar button1 nospiešanu
    {
      motState=0;// nonullē mainīgo
      if (senLeft) {led1ON;
        motState=motState+1;
      }else{led1OFF;}//pieskaitam motoru stavoklim sensora binaro vertibu
      if (senRight) {led2ON;
        motState=motState+2;
      }else{led2OFF;}
      if (senBack) {led3ON;
        motState=motState+4;
      }else{led3OFF;}

      if (motState==0){//SHARP sesnorus saktisies tikai tad ja nav nostradajis neviens linijas sensors
        if (SHARP1) {
          motState=64;
        }
        if (SHARP5) {
          motState=128;
        }
        if (SHARP2) {
          motState=motState+8;
        }
        if (SHARP3) {
          motState=motState+16;
        }
        if (SHARP4) {
          motState=motState+32;
        }

      }
      //motState tagad glabā sensoru stāvokļa vērtību
      switch (motState){//Switch struktūra nolasa mainīgo
      case 0://neviens nav nostradajis
        Forward(120,120);
        break;
      case 1: //kreisais sensors
        Backward(210,210);
        delay(100);
        Right(200,200);
        delay(100);
        break;
      case 2://labais sensors
        Backward(210,210);
        delay(100);
        Left(200,200);
        delay(100);
        break;
      case 3://abi prieksejie
        Backward(220,220);
        delay(200);
        break;
      case 4://aizmugurejais
        Forward(255,255);
        delay(200);
        break;
      case 5://aizmugurejais un kreisais
        Right (200,200);
        delay(100);
        Forward(150,150);
        delay(100);
        break;
      case 6://aizmugurejais un labais
        Left (200,200);
        delay(100);
        Forward(150,150);
        delay(100);
        break;
      case 7://visi sensori nostradajusi
        Forward(0,0);
        break;

        // ja robotam ir nostradajusi SHARP sensori (prieksejie 3)
      case 8://SHARP2
        Left(150,150);
        break;
      case 16://SHARP3
        Forward(200,200);
        break;
      case 24://SHARP2 & SHARP3
        Left(150,150);
        break;
      case 32://SHARP4
        Right(150,150);
        break;
      case 40://SHARP4 &SHARP2
        Forward(150,150);
        break;
      case 48://SHARP4 & SHARP3
        Right(150,150);
        break;
      case 56://SHARP2 &SHARP3 &SHARP4
        Forward(255,255);
        break;
        case 64://SHARP2 &SHARP3 &SHARP4
        Left(200,200);
        break;
        case 128://SHARP2 &SHARP3 &SHARP4
        Right(200,200);
        break;
      }   
    }
    while (button1){ //Cikls, kamēr poga ir nospiesta
    }
  }
}  













