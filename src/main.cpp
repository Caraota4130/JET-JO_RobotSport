#include <Arduino.h>
#include <SimpleVector.h>
#include <Pixy2/Pixy2.h>
#include <classes.h>


Movimiento Robot(2, 4, 9, 7, 8, 10);
//Podadora Pd(12, 13);
//Camara Pixy;

void setup() {
  Serial.begin(9600);
  //Robot.iniciar();
  //Pd.iniciar();
}

void loop() {
  Robot.avanzar(180, 185, 1000);
  Robot.detenerse();
  delay(500);
  Robot.retroceder(210, 180, 2000);
  Robot.avanzar(180, 200, 160);
  Robot.girarDerecha(180, 200, 600);
  Robot.avanzar(180, 185, 700);
  Robot.retroceder(180, 200, 200);
  Robot.girarIzquierda(180, 200, 600);
  Robot.retroceder(180, 200, 600);
  delay(3000);
}

