#include <Arduino.h>
#include "rgb_lcd.h"
#include <ESP32Encoder.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SPI.h>


rgb_lcd lcd;



int BP0 = 0;
int BP1 = 2;
int BP2 = 12;
int POT = 33;
int pmw = 27;
int EA = 23 ;
int EB = 19 ;

int Val_BP0 ;
int Val_BP1 ;
int Val_BP2 ;
int Val_POT ;


int frequence = 25000;
int canal1 = 1;
int resolution = 11;
int phase = 26 ;
int couleur; 
int vit;

uint16_t r, g, b, c;
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_16X);
  
ESP32Encoder encoder;

void setup()
{
  // Initialise la liaison avec le terminal
  Serial.begin(115200);
  
  
  // Initialise l'écran LCD
  Wire1.setPins(15, 5);
  lcd.begin(16, 2, LCD_5x8DOTS, Wire1);
  /*lcd.printf("Trieur de balles\n");*/
  lcd.setRGB(0,0,255);

  ledcSetup(canal1,frequence,resolution);

  pinMode(BP0,INPUT_PULLUP);
  pinMode(BP1,INPUT_PULLUP);
  pinMode(BP2,INPUT_PULLUP);
  pinMode(phase,OUTPUT);

  ledcAttachPin(pmw,canal1);

  if (tcs.begin()){
      Serial.println("TCS detecter!");
  }
  else{
      Serial.println("TCS pas detecter!");
  }

  encoder.attachFullQuad(EA,EB);
  encoder.setCount(0);
}

void loop()
{
  
  vit = encoder.getCount();
  Val_POT = analogRead(POT);
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
  lcd.setCursor(0,0);
  /*lcd.printf("BP0%dBP1%dBP2%d",Val_BP0,Val_BP1,Val_BP2);*/
  /*lcd.printf("POT%4d",Val_POT);*/
  /*Serial.printf("POT%4d",Val_POT);*/

  tcs.getRawData(&r,&g,&b,&c);
  couleur = tcs.calculateColorTemperature(r,g,b);

  lcd.printf("R:%5d", r);
  lcd.printf("G:%5d", g);
  lcd.setCursor(0,1);
  lcd.printf("B:%5d", b);
  lcd.printf("C:%5d", c);
  

  Serial.printf("Top : %d\n", vit);
}
