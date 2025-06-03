#include <Arduino.h>
#include <SimpleVector.h>
#include <Pixy2/Pixy2.h>


const int GIRO = 1000;
int timing = 0;

class Movimiento
{
private:
  int MD[2], MI[2];
  int velocidad;

public:
  Movimiento(int MDa, int MDb, int MIa, int MIb, int velocidad = 255)
  {
    this->MD[0] = MDa;
    this->MD[1] = MDb;
    this->MI[0] = MIa;
    this->MI[1] = MIb;
    this->velocidad = velocidad;
  };
  void iniciar()
  {
    // Configuracion de pines
    pinMode(this->MD[0], OUTPUT);
    pinMode(this->MD[1], OUTPUT);
    pinMode(this->MI[0], OUTPUT);
    pinMode(this->MI[1], OUTPUT);

    // Apagar motores
    analogWrite(this->MD[0], 0);
    analogWrite(this->MD[1], 0);
    analogWrite(this->MI[0], 0);
    analogWrite(this->MI[1], 0);
  };
  void setVelocidad(int num)
  {
    this->velocidad = num;
  };
  void detenerse()
  {
    analogWrite(this->MD[0], 0);
    analogWrite(this->MD[1], 0);
    analogWrite(this->MI[0], 0);
    analogWrite(this->MI[1], 0);
  };
  void avanzar(int tiempo = 1000)
  {
    analogWrite(this->MD[0], this->velocidad);
    analogWrite(this->MD[1], 0);
    analogWrite(this->MI[0], this->velocidad);
    analogWrite(this->MI[1], 0);
    delay(tiempo);
    this->detenerse();
  };
  void retroceder(int tiempo = 1000)
  {
    analogWrite(this->MD[0], 0);
    analogWrite(this->MD[1], this->velocidad);
    analogWrite(this->MI[0], 0);
    analogWrite(this->MI[1], this->velocidad);
    delay(tiempo);
    this->detenerse();
  };
  void girarIzquierda(int tiempo = GIRO)
  {
    analogWrite(this->MD[0], 0);
    analogWrite(this->MD[1], this->velocidad);
    analogWrite(this->MI[0], this->velocidad);
    analogWrite(this->MI[1], 0);
    delay(tiempo);
    this->detenerse();
  };
  void girarDerecha(int tiempo = GIRO)
  {
    analogWrite(this->MD[0], this->velocidad);
    analogWrite(this->MD[1], 0);
    analogWrite(this->MI[0], 0);
    analogWrite(this->MI[1], this->velocidad);
    delay(tiempo);
    this->detenerse();
  };
};

class Camara {
  
  public:

    Pixy2 pixy;
    int numBloques;
    SimpleVector<int> Sizes;
    
    Camara() {
      this->pixy.init();
      this->pixy.setCameraBrightness(100);
      this->pixy.setLamp(1, 1);
    }

    void close() {
      this->Sizes.releaseMemory();
    }

    void getBloques() {
      this->pixy.ccc.getBlocks();
      this->numBloques=this->pixy.ccc.numBlocks;
      for (int i = 0; i < numBloques; i++)
      {
        if (pixy.ccc.blocks[i].m_age >= 45)
        {
          this->Sizes.push_back(this->pixy.ccc.blocks[i].m_width*this->pixy.ccc.blocks[i].m_height);
        }
      }
       
    }

    void infoBloques() {
      for (int i = 0; i < numBloques; i++)
      {
        Serial.print("Bloque: ");
        Serial.println(i+1);
        Serial.print("Tamanio: ");
        Serial.print(Sizes.get(i));
        Serial.print("\tPosicion X: ");
        Serial.print(this->pixy.ccc.blocks[i].m_x);
        Serial.print("\tPosicion Y: ");
        Serial.print(this->pixy.ccc.blocks[i].m_y);
        Serial.print("\tAncho: ");
        Serial.print(pixy.ccc.blocks[i].m_width);
        Serial.print("\tAlto: ");
        Serial.print(pixy.ccc.blocks[i].m_height);
        Serial.print("\tDistancia: ");
        this->switchDist(Sizes.get(i));
        Serial.print("\n\n");
      }
    }

  private:
    float Prom_Sizes() {
      float promSize;
      SimpleVector<int>::SimpleVectorIterator Size = this->Sizes.begin(); 
      while (Size.hasNext())
      {
        promSize += Size.next();
      }
      promSize /= numBloques;
      return promSize;
    }

    int switchDist(int size) {
      float promSize = Prom_Sizes();
      if (size > 1 && size < promSize/2)
      {
        Serial.print("Cercano");
      } else if(size > promSize / 2 && size < promSize) {
        Serial.print("Mediano");
      } else if(size > promSize && size < promSize * 2) {
        Serial.print("Lejano");
      }
      return 2;
    }
    
};

Movimiento Robot(3, 5, 9, 10);
Camara Pixy;

void setup() {
  Serial.begin(9600);
  Robot.iniciar();
}

void loop() {
  Serial.println();
  Pixy.getBloques();
  Pixy.infoBloques();

  Robot.avanzar();
  Robot.retroceder();
  Robot.girarDerecha();
  Robot.girarIzquierda();
  

  Pixy.close();
}

