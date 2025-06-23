#include <Arduino.h>
#include <Wire.h>
#include <SimpleVector.h>
#include <Pixy2/Pixy2.h>
#include <classes.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Movimiento Robot(2, 4, 9, 7, 8, 10);
// Podadora Pd(12, 13);
// Camara Pixy;

void setup()
{
  Serial.begin(9600);
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Robot.iniciar();
  // Pd.iniciar();
  Robot.avanzar(180, 185, 300);
}

void loop()
{
  while (digitalRead(13)!=1 && digitalRead(12)!=1)
  {
    Robot.avanzarCiclo(180, 185);
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Blanco");
    display.display();
  }
  Robot.detenerse();
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Negro");
  display.display();
  delay(2000);
}