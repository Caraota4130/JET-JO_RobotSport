#include <Arduino.h>
#include <SimpleVector.h>
#include <Pixy2/Pixy2.h>


const int giro = 180;
int timing=1000;

class Movimiento {
	private:
  	int MD[2], MI[2], EN[2];
    int velocidad;
  public:
    Movimiento(int MIa, int MIb, int ENI, int MDa, int MDb, int END, int velocidad=250) {
      this->MD[0]=MIa;
      this->MD[1]=MIb;
      this->EN[0]=ENI;
      this->MI[0]=MDa;
      this->MI[1]=MDb;
      this->EN[1]=END;
      this->velocidad=velocidad;
    };
    void iniciar() {
      //Configuracion de pines
      pinMode(this->MD[0], OUTPUT);
	    pinMode(this->MD[1], OUTPUT);
      pinMode(this->MI[0], OUTPUT);
	    pinMode(this->MI[1], OUTPUT);

      //Apagar motores
      analogWrite(this->EN[0], 0);
      analogWrite(this->EN[1], 0);
	    digitalWrite(this->MD[0], 0);
	    digitalWrite(this->MD[1], 0);
	    digitalWrite(this->MI[0], 0);
	    digitalWrite(this->MI[1], 0);
    };	
  	void setVelocidad(int num){
      this->velocidad=num;
    };
    void detenerse() {
      analogWrite(this->EN[0], 0);
      analogWrite(this->EN[1], 0);
      digitalWrite(this->MD[0], 0);
	    digitalWrite(this->MD[1], 0);
	    digitalWrite(this->MI[0], 0);
	    digitalWrite(this->MI[1], 0);
    };
    void avanzar(int tiempo=1000) {
      analogWrite(this->EN[0], this->velocidad);
      analogWrite(this->EN[1], this->velocidad);
      digitalWrite(this->MD[0], 1);
	    digitalWrite(this->MD[1], 0);
	    digitalWrite(this->MI[0], 1);
	    digitalWrite(this->MI[1], 0);
      delay(tiempo);
      this->detenerse();
    };
		void retroceder(int tiempo=1000) {
      analogWrite(this->EN[0], this->velocidad);
      analogWrite(this->EN[1], this->velocidad);
      digitalWrite(this->MD[0], 0);
	    digitalWrite(this->MD[1], 1);
	    digitalWrite(this->MI[0], 0);
	    digitalWrite(this->MI[1], 1);
      delay(tiempo);
      this->detenerse();
    };
  	void girarIzquierda(int tiempo=giro) {
      digitalWrite(this->MD[0], 0);
	    digitalWrite(this->MD[1], 1);
	    digitalWrite(this->MI[0], 1);
	    digitalWrite(this->MI[1], 0);
      delay(tiempo);
      this->detenerse();
    };
    void girarDerecha() {
      digitalWrite(this->MD[0], 1);
	    digitalWrite(this->MD[1], 0);
	    digitalWrite(this->MI[0], 0);
	    digitalWrite(this->MI[1], 1);
    };
};

class Podadora {
  private:
    int pines[2];
    int fuerza;
  public:
    Podadora(int PinA, int PinB, int fuerza=255) {
      this->pines[0]=PinA;
      this->pines[1]=PinB;
      this->fuerza=fuerza;
    }
    void iniciar() {
      pinMode(this->pines[0], OUTPUT);
      pinMode(this->pines[1], OUTPUT);

      analogWrite(this->pines[0], 0);
      analogWrite(this->pines[1], 0);
    }
    void setFuerza(int num) {
      this->fuerza = num;
    }
    void detener() {
      analogWrite(this->pines[0], 0);
      analogWrite(this->pines[1], 0);
    }
    void girarNegativo(int tiempo = 1000) {
      analogWrite(this->pines[0], this->fuerza);
      analogWrite(this->pines[1], 0);
      delay(1000);
      this->detener();
    }
    void girarPositivo(int tiempo = 1000) {
      analogWrite(this->pines[0], 0);
      analogWrite(this->pines[1], this->fuerza);
      delay(1000);
      this->detener();
    }
  private:

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
      this->Sizes.clear();
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

    /*

    void centerObject(Movimiento Robot) {
      pixy.getResolution();
      Robot->setVelocidad(140);
      Serial.println("Centrando objeto...");
      do
      {
        this->positionX_Object();
        Robot->girarDerecha();
        if(pixy.ccc.blocks[0].m_x > pixy.frameWidth - Sizes.get(0) && pixy.ccc.blocks[0].m_x < pixy.frameWidth + Sizes.get(0)) {
          Serial.println("Objeto centrado");
          Robot->detenerse();
          break;
        }
      } while (pixy.ccc.blocks[0].m_x < pixy.frameWidth / 2);
      Serial.println("Terminando funcion...");
    }
    */

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
      float promSize = 0;
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
      if (size > 1 && size < promSize/2) {
        Serial.print("Lejano\t");
      } else if(size > promSize / 2 && size < promSize) {
        Serial.print("Mediano\t");
      } else if(size >= promSize) {
        Serial.print("Cercano\t");
      }
      Serial.print("Promedio de Distancia: ");
      Serial.println(promSize);
      return 2;
    }

    void positionX_Object() {
      Serial.print("\nX:\t");
      Serial.print(pixy.ccc.blocks[0].m_x);
      if(pixy.ccc.blocks[0].m_x > pixy.frameWidth / 2) {
        Serial.println("Esta a la derecha");
      } else if(pixy.ccc.blocks[0].m_x < pixy.frameWidth / 2) {
        Serial.println("Esta a la izquierda");
      }

    }

    
};

Movimiento Robot(2, 4, 9, 7, 8, 10, 200);
Podadora Pd(12, 13);
//Camara Pixy;

void setup() {
  Serial.begin(9600);
  Robot.iniciar();
  Pd.iniciar();
}

void loop() {
  //Pixy.getBloques();
//HOLAA
  //Pd.girarPositivo();
  Robot.avanzar();
  //Pd.girarPositivo();
  //Pixy.centerObject(Robot);

  //Pixy.close();
}

