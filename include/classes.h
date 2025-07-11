class Movimiento
{
private:
    int MD[2], MI[2], EN[2];

public:
    Movimiento(int MIa, int MIb, int ENI, int MDa, int MDb, int END)
    {
        this->MD[0] = MIa;
        this->MD[1] = MIb;
        this->EN[0] = ENI;
        this->MI[0] = MDa;
        this->MI[1] = MDb;
        this->EN[1] = END;
    };
    void iniciar()
    {
        // Configuracion de pines
        pinMode(this->MD[0], OUTPUT);
        pinMode(this->MD[1], OUTPUT);
        pinMode(this->MI[0], OUTPUT);
        pinMode(this->MI[1], OUTPUT);

        // Apagar motores
        analogWrite(this->EN[0], 0);
        analogWrite(this->EN[1], 0);
        digitalWrite(this->MD[0], 0);
        digitalWrite(this->MD[1], 0);
        digitalWrite(this->MI[0], 0);
        digitalWrite(this->MI[1], 0);
    };
    void detenerse()
    {
        analogWrite(this->EN[0], 0);
        analogWrite(this->EN[1], 0);
        digitalWrite(this->MD[0], 0);
        digitalWrite(this->MD[1], 0);
        digitalWrite(this->MI[0], 0);
        digitalWrite(this->MI[1], 0);
    };
    void avanzar(int vel1, int vel2, int tiempo = 1000)
    {
        analogWrite(this->EN[0], vel1);
        analogWrite(this->EN[1], vel2);
        digitalWrite(this->MD[0], 1);
        digitalWrite(this->MI[0], 1);
        digitalWrite(this->MD[1], 0);
        digitalWrite(this->MI[1], 0);
        delay(tiempo);
        this->detenerse();
    };
    void avanzarCiclo(int vel1, int vel2)
    {
        analogWrite(this->EN[0], vel1);
        analogWrite(this->EN[1], vel2);
        digitalWrite(this->MD[0], 1);
        digitalWrite(this->MI[0], 1);
        digitalWrite(this->MD[1], 0);
        digitalWrite(this->MI[1], 0);
    };
    void retroceder(int vel1, int vel2, int tiempo = 1000)
    {
        analogWrite(this->EN[0], vel1);
        analogWrite(this->EN[1], vel2);
        digitalWrite(this->MD[0], 0);
        digitalWrite(this->MI[0], 0);
        digitalWrite(this->MI[1], 1);
        digitalWrite(this->MD[1], 1);
        delay(tiempo);
        this->detenerse();
    };
    void retrocederCiclo(int vel1, int vel2)
    {
        analogWrite(this->EN[0], vel1);
        analogWrite(this->EN[1], vel2);
        digitalWrite(this->MD[0], 0);
        digitalWrite(this->MI[0], 0);
        digitalWrite(this->MI[1], 1);
        digitalWrite(this->MD[1], 1);
    };
    void girarIzquierda(int vel1, int vel2, int giro)
    {
        analogWrite(this->EN[0], vel1);
        analogWrite(this->EN[1], vel2);
        digitalWrite(this->MD[1], 1);
        digitalWrite(this->MI[0], 1);
        digitalWrite(this->MD[0], 0);
        digitalWrite(this->MI[1], 0);
        delay(giro);
        this->detenerse();
    };
    void girarDerecha(int vel1, int vel2, int giro)
    {
        analogWrite(this->EN[0], vel1);
        analogWrite(this->EN[1], vel2);
        digitalWrite(this->MD[0], 1);
        digitalWrite(this->MI[1], 1);
        digitalWrite(this->MD[1], 0);
        digitalWrite(this->MI[0], 0);
        delay(giro);
        this->detenerse();
    };
};

class Podadora
{
private:
    int M1[2];
    int fuerza;

public:
    Podadora(int M1A, int M1B, int fuerza = 255)
    {
        this->M1[0] = M1A;
        this->M1[1] = M1B;

        pinMode(this->M1[0], OUTPUT);
        pinMode(this->M1[1], OUTPUT);

        analogWrite(this->M1[0], 0);
        analogWrite(this->M1[1], 0);
        this->fuerza = fuerza;
    }
    void iniciar()
    {
        
    }
    void setFuerza(int num)
    {
        this->fuerza = num;
    }
    void detener()
    {
        analogWrite(this->M1[0], 0);
        analogWrite(this->M1[1], 0);
    }
    void girarNegativo(int tiempo = 1000)
    {
        analogWrite(this->M1[0], this->fuerza);
        analogWrite(this->M1[1], 0);
        delay(tiempo);
        this->detener();
    }
    void girarPositivo(int tiempo = 1000)
    {
        analogWrite(this->M1[0], 0);
        analogWrite(this->M1[1], this->fuerza);
        delay(tiempo);
        this->detener();
    }

private:
};

class Camara
{

public:
    Pixy2 pixy;
    int numBloques;
    SimpleVector<int> Sizes;

    Camara()
    {
        this->pixy.init();
        this->pixy.setCameraBrightness(100);
        this->pixy.setLamp(1, 1);
    }

    void close()
    {
        this->Sizes.clear();
        this->Sizes.releaseMemory();
    }

    void getBloques()
    {
        this->pixy.ccc.getBlocks();
        this->numBloques = this->pixy.ccc.numBlocks;
        for (int i = 0; i < numBloques; i++)
        {
            if (pixy.ccc.blocks[i].m_age >= 45)
            {
                this->Sizes.push_back(this->pixy.ccc.blocks[i].m_width * this->pixy.ccc.blocks[i].m_height);
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

    void infoBloques()
    {
        for (int i = 0; i < numBloques; i++)
        {
            Serial.print("Bloque: ");
            Serial.println(i + 1);
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
    float Prom_Sizes()
    {
        float promSize = 0;
        SimpleVector<int>::SimpleVectorIterator Size = this->Sizes.begin();
        while (Size.hasNext())
        {
            promSize += Size.next();
        }
        promSize /= numBloques;
        return promSize;
    }

    int switchDist(int size)
    {
        float promSize = Prom_Sizes();
        if (size > 1 && size < promSize / 2)
        {
            Serial.print("Lejano\t");
        }
        else if (size > promSize / 2 && size < promSize)
        {
            Serial.print("Mediano\t");
        }
        else if (size >= promSize)
        {
            Serial.print("Cercano\t");
        }
        Serial.print("Promedio de Distancia: ");
        Serial.println(promSize);
        return 2;
    }

    void positionX_Object()
    {
        Serial.print("\nX:\t");
        Serial.print(pixy.ccc.blocks[0].m_x);
        if (pixy.ccc.blocks[0].m_x > pixy.frameWidth / 2)
        {
            Serial.println("Esta a la derecha");
        }
        else if (pixy.ccc.blocks[0].m_x < pixy.frameWidth / 2)
        {
            Serial.println("Esta a la izquierda");
        }
    }
};