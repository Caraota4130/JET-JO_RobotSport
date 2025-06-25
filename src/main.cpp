#include <Arduino.h>
#include <Wire.h>
#include <SimpleVector.h>
#include <Pixy2/Pixy2.h>
#include <classes.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define IR_IZQUIERDA 12
#define IR_DERECHA 13
#define IR_ATRAS 11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Movimiento Robot(2, 4, 9, 7, 8, 10);
Podadora Pd(6, 5, 255);

void hastaLinea() {
  Robot.avanzarCiclo(220, 240);
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Blanco");
    display.display();
  while (true) {
    if (digitalRead(IR_IZQUIERDA) == 1 && digitalRead(IR_DERECHA) == 1) {
      break;
    }
  }
  Robot.detenerse();
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Negro");
    display.display();
  delay(700);
}

void hastaPared() {
  Robot.retrocederCiclo(180, 177);
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Sin pared");
    display.display();
  while (true) {
    if (digitalRead(IR_ATRAS) != 1) {
      break;
    }
  }
  Robot.detenerse();
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Pared");
    display.display();
  delay(700);
}

void setup()
{
  Serial.begin(9600);
  pinMode(IR_IZQUIERDA, INPUT);
  pinMode(IR_DERECHA, INPUT);
  pinMode(IR_ATRAS, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  
}

void loop()
{
  //-----------------CARRIL 1---------------------------//
  Robot.avanzar(180, 180, 300);
  hastaLinea();

  Robot.avanzar(220, 240, 70);
  Pd.girarPositivo(1000);

  hastaPared();
  
  //--------------------------------------------------//

  //-------------CAMBIO DE CARRIL IZ-DR---------------------//
  Robot.avanzar(180, 200, 160);
  Robot.girarDerecha(180, 200, 600);
  Robot.avanzar(180, 185, 400);
  Robot.retroceder(180, 200, 200);
  Robot.girarIzquierda(180, 200, 500);
  hastaPared();
  //-------------------------------------------------//

  //------------------CARRIL 2-----------------------//
  Robot.avanzar(180, 180, 300);
  
  hastaLinea();

  hastaPared();

  //-----------------------------------------------//

  //-------------CAMBIO DE CARRIL DR-IZ---------------------//
  Robot.avanzar(180, 200, 160);
  Robot.girarIzquierda(180, 200, 600);
  Robot.retroceder(180, 200, 200);
  Robot.avanzar(180, 185, 400);
  Robot.girarDerecha(180, 200, 550);
  hastaPared();
  //-------------------------------------------------//
  delay(3000);
}
