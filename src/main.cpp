#include <Arduino.h>
#include "rgb_lcd.h"
#include <ESP32Encoder.h>

rgb_lcd lcd;


  int BP0 = 0;
  int BP1 = 2;
  int BP2 = 12;
  int POT = 33;
  int pmw = 27;


  int Val_BP0 ;
  int Val_BP1 ;
  int Val_BP2 ;
  int Val_POT ;


  int frequence = 25000;
  int canal1 = 1;
  int resolution = 11;

  

void setup()
{
  // Initialise la liaison avec le terminal
  Serial.begin(9600);

  // Initialise l'écran LCD
  Wire1.setPins(15, 5);
  lcd.begin(16, 2, LCD_5x8DOTS, Wire1);
  lcd.printf("Trieur de balles\n");
  lcd.setRGB(0,0,255);

  ledcSetup(canal1,frequence,resolution);

  pinMode(BP0,INPUT_PULLUP);
  pinMode(BP1,INPUT_PULLUP);
  pinMode(BP2,INPUT_PULLUP);

  ledcAttachPin(pmw,canal1);


}

void loop()
{
  Val_POT = analogRead(POT);
  Val_BP0 = digitalRead(BP0);
  Val_BP1 = digitalRead(BP1);
  Val_BP2 = digitalRead(BP2);
  ledcWrite(canal1,Val_POT/2);
  lcd.setCursor(0,0);
  lcd.printf("BP0%dBP1%dBP2%d",Val_BP0,Val_BP1,Val_BP2);
  lcd.setCursor(0,1);
  lcd.printf("POT%4d",Val_POT);
  Serial.printf("POT%4d",Val_POT);
}
