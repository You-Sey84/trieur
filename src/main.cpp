#include <Arduino.h>
#include "rgb_lcd.h"

rgb_lcd lcd;


  int BP0 = 0;
  int BP1 = 2;
  int BP2 = 12;
  int POT = 33;

  int Val_BP0 ;
  int Val_BP1 ;
  int Val_BP2 ;
  int Val_POT ;

void setup()
{
  // Initialise la liaison avec le terminal
  Serial.begin(9600);

  // Initialise l'écran LCD
  Wire1.setPins(15, 5);
  lcd.begin(16, 2, LCD_5x8DOTS, Wire1);
  lcd.printf("Trieur de balles\n");
  lcd.setRGB(0,0,255);

  pinMode(BP0,INPUT_PULLUP);
  pinMode(BP1,INPUT_PULLUP);
  pinMode(BP2,INPUT_PULLUP);

}

void loop()
{
  Val_POT = analogRead(POT);
  Val_BP0 = digitalRead(BP0);
  Val_BP1 = digitalRead(BP1);
  Val_BP2 = digitalRead(BP2);
  lcd.setCursor(0,0);
  lcd.printf("BP0%dBP1%dBP2%d",Val_BP0,Val_BP1,Val_BP2);
  lcd.setCursor(0,1);
  lcd.printf("POT%5d",Val_POT);
  Serial.printf("POT%5d",Val_POT);

}
