#include <Arduino.h>
#include "rgb_lcd.h"
#include <ESP32Encoder.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define Kp  20
#define Ki  4


rgb_lcd lcd;


int BP0 = 0;
int BP1 = 2;
int BP2 = 12;
int POT = 33;
int pmw = 27;
int servo = 13;
int EA = 23 ;
int EB = 19 ;
int IR0 = 36;


int Val_BP0 ;
int Val_BP1 ;
int Val_BP2 ;
int Val_POT ;
int Val_POS = 4700 ; //  3700   4700init   5700
int Val_IR0 ;
int Val_PWM = 50;
int erreur ,  somme;
int csg = 10;  // changer consigne
int vit ,pos;


int frequence = 25000;
int frequence2 = 50;
int canal1 = 1;
int canal2 = 2;
int resolution = 11;
int resolution1 = 16;
int phase = 26 ;
int couleur; 
int etat = 0;

uint16_t r, g, b, c;
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_16X);
  
ESP32Encoder encoder;
Adafruit_MPU6050 mpu;

void vTaskPeriodic(void *pvParameters)
{
  int64_t a, aprec;
  TickType_t xLastWakeTime;
  // Lecture du nombre de ticks quand la tâche commence
  xLastWakeTime = xTaskGetTickCount();
  while (1)
  {
  /*Serial.printf("A répéter\n");*/
 
  // Endort la tâche pendant le temps restant par rapport au réveil,
  // ici 100ms, donc la tâche s'effectue ici toutes les 100ms.
  // xLastWakeTime sera mis à jour avec le nombre de ticks au prochain
  // réveil de la tâche.
  
  a = encoder.getCount();
  vit = a - aprec;
  aprec = a ;
  erreur = csg - vit;
  somme = somme + erreur;
  if (somme < -100) somme = -100;
  if (somme > 100)  somme = 100;
  Val_PWM = Kp * erreur + Ki * somme;
  if(Val_PWM>0)
  {
    if (Val_PWM >2047)
    {
      Val_PWM = 2047;
    }
    digitalWrite(phase , LOW);
    ledcWrite(canal1, Val_PWM);
  }
  else{
    if (Val_PWM < -2047)
    {
      Val_PWM = -2047;
    }
    digitalWrite(phase , HIGH);
    ledcWrite(canal1, -Val_PWM);
  }
  
  vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));



 }
}

void setup()
{
  // Initialise la liaison avec le terminal
  Serial.begin(115200);



  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  
  // Initialise l'écran LCD
  Wire1.setPins(15, 5);
  lcd.begin(16, 2, LCD_5x8DOTS, Wire1);
  /*lcd.printf("Trieur de balles\n");*/
  lcd.setRGB(0,0,255);

  /*if (tcs.begin()){
      Serial.println("TCS detecter!");
  }
  else{
      Serial.println("TCS pas detecter!");
  }*/

  ledcSetup(canal1,frequence,resolution);
  ledcSetup(canal2,frequence2,resolution1);

  ledcAttachPin(pmw,canal1);
  ledcAttachPin(servo,canal2);

  pinMode(BP0,INPUT_PULLUP);
  pinMode(BP1,INPUT_PULLUP);
  pinMode(BP2,INPUT_PULLUP);
  pinMode(phase,OUTPUT);


  encoder.attachFullQuad(EA,EB);
  

  
  
  xTaskCreate(vTaskPeriodic, "vTaskPeriodic", 10000, NULL, 2, NULL);
  ledcWrite(canal2,Val_POS);   // 3700   4700   5700
  csg = 5;
  while(Val_IR0 < 2000){
    Val_IR0 = analogRead(IR0);
  }
}

void loop()
{
  
  pos = encoder.getCount();
  Val_POT = analogRead(POT);
  Val_IR0 = analogRead(IR0);
  Val_BP0 = digitalRead(BP0);
  Val_BP1 = digitalRead(BP1);
  Val_BP2 = digitalRead(BP2); 
  ledcWrite(canal2,Val_POS);   // 3700   4700   5700

  switch (etat)
  {
  case 0 :
    delay(2000);
    csg = 0;
    
    if (Val_BP0 == 0 ) etat = 2;
    break;
  case 2 :
    encoder.setCount(0);
    delay(2000);
    csg = 10;
    if (pos == 560 ) etat = 3;
    break;
  case 3 :
    csg = 0;
    delay(2000);
    if (r>5000 & g>5000 & b>5000) etat = 4;
    else etat = 5;
    break;
  case 4 :
    delay(2000);
    csg = 10;
    
    if (pos == 1080) etat = 6;
    break;
  case 5 :
    delay(2000);
    csg = 10;
    
    if (pos == 880) etat = 6;
    break;
  case 6 :
    csg = 0;
    delay(2000);
    etat = 7;
    break;
  case 7 :
    delay(2000);
    csg = 10;
    if (pos == 1600) etat = 8;
    break;
  case 8 :
    delay(2000);
    csg = 0;
    if (Val_IR0 > 2000) etat = 9;
    else etat = 10;
    break;
  case 9 :
    etat = 0;
    break;
  case 10 :
    etat = 0;
    break;
  default:
    etat = 0;
    break;

  }

  
  lcd.setCursor(0,0);
  lcd.printf("etat = %d", etat);
  Serial.printf("etat = %d", etat);
  lcd.setCursor(0,1);
  lcd.printf("cny = %d", Val_IR0);
  Serial.printf("cny = %d", Val_IR0);
  /*
  tcs.getRawData(&r,&g,&b,&c);
  couleur = tcs.calculateColorTemperature(r,g,b);

  
  Serial.printf("R:%5d", r);
  Serial.printf("G:%5d", g);
  Serial.printf("B:%5d", b);
  Serial.printf("C:%5d\n", c);
  

  Serial.printf("Top : %d\n", pos);
  static int i = 0;
  Serial.printf("Boucle principale : %d\n", i++);
  delay(1000); 
  
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  
  Serial.print("AccelX:");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print("AccelY:");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print("AccelZ:");
  Serial.print(a.acceleration.z);
  Serial.print(", ");
  Serial.print("GyroX:");
  Serial.print(g.gyro.x);
  Serial.print(",");
  Serial.print("GyroY:");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.print("GyroZ:");
  Serial.print(g.gyro.z);
  Serial.println("");*/
}
