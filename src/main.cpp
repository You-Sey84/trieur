#include <Arduino.h>
#include "rgb_lcd.h"
#include <ESP32Encoder.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>



void vTaskPeriodic(void *pvParameters)
{
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

 vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));



 }
}
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

int frequence = 25000;
int frequence2 = 50;
int canal1 = 1;
int canal2 = 2;
int resolution = 11;
int resolution1 = 16;
int phase = 26 ;
int couleur; 
int vit;

uint16_t r, g, b, c;
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_16X);
  
ESP32Encoder encoder;
Adafruit_MPU6050 mpu;

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
  xTaskCreate(vTaskPeriodic, "vTaskPeriodic", 10000, NULL, 2, NULL);
  


  
  // Initialise l'écran LCD
  Wire1.setPins(15, 5);
  lcd.begin(16, 2, LCD_5x8DOTS, Wire1);
  /*lcd.printf("Trieur de balles\n");*/
  lcd.setRGB(0,0,255);



  if (tcs.begin()){
      Serial.println("TCS detecter!");
  }
  else{
      Serial.println("TCS pas detecter!");
  }



  ledcSetup(canal1,frequence,resolution);
  ledcSetup(canal2,frequence2,resolution1);

  ledcAttachPin(pmw,canal1);
  ledcAttachPin(servo,canal2);



  pinMode(BP0,INPUT_PULLUP);
  pinMode(BP1,INPUT_PULLUP);
  pinMode(BP2,INPUT_PULLUP);
  pinMode(phase,OUTPUT);


  encoder.attachFullQuad(EA,EB);
  encoder.setCount(0);

  lcd.setCursor(0,0);
  
}

void loop()
{
  
  vit = encoder.getCount();
  Val_POT = analogRead(POT);
  Val_IR0 = analogRead(IR0);
  Val_BP0 = digitalRead(BP0);
  Val_BP1 = digitalRead(BP1);
  Val_BP2 = digitalRead(BP2); 

  if(Val_BP0 == HIGH)
  {
    digitalWrite(phase,HIGH);

  }

  else{
    digitalWrite(phase,LOW
    
    );
  }
  
  ledcWrite(canal1,Val_POT/2);
  ledcWrite(canal2,Val_POS);   // 3700   4700   5700
  
  lcd.setCursor(0,0);
  /*lcd.printf("BP0%dBP1%dBP2%d",Val_BP0,Val_BP1,Val_BP2);*/
  lcd.printf("POT%10d\n",Val_POT*4);
  /*Serial.printf("POT%4d",Val_POT);*/

  tcs.getRawData(&r,&g,&b,&c);
  couleur = tcs.calculateColorTemperature(r,g,b);

  /*
  lcd.printf("R:%5d", r);
  lcd.printf("G:%5d", g);
  lcd.setCursor(0,1);
  lcd.printf("B:%5d", b);
  lcd.printf("C:%5d", c);*/
  

  /*Serial.printf("Top : %d\n", vit);
  static int i = 0;
  Serial.printf("Boucle principale : %d\n", i++);
  delay(1000); 
  */

   if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
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
    Serial.println("");
  }
}
