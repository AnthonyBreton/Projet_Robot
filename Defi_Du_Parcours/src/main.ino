/*
Projet: Combattant A pas manette
Equipe: Laurent Ben
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

#include <LibRobus.h> // Essentielle pour utiliser RobUS


#define PI 3.14159265359

void Avancer(float distance);
float PID(int pulseM, int pulseE, float vitesseM, long pulseT, long totalE);



void setup(){
  BoardInit();
}


void loop() {
   if(ROBUS_IsBumper(0) + ROBUS_IsBumper(1) + ROBUS_IsBumper(2) + ROBUS_IsBumper(3) > 0)
  {
   delay(60000);
   
  }
}
void pince(int position)
{
  if(position==1) {
SERVO_Enable(1);
SERVO_SetAngle(1,160);
  }
    if(position==0) {
SERVO_Enable(1);
SERVO_SetAngle(1,70);
  }
  else
  {
    SERVO_Disable(0);
  }
  
  
}
/*void fonctionbleu()
{
  tourner(90);
  delay(50);

  Avancer(43.5);
  delay(50);

  tourner(45);
  delay(50);

  Avancer(66);
  delay(50);

  pince(1);//fermer les pinces
  delay(50);

  tourner(180);
  delay(50);

  Avancer(111);
  delay(50);

  pince(0);//ouvrir les pinces
  delay(50);

  tourner(135);
  delay(50);

  avancer(100);
  delay(50);
}

void fonctionjaune()
{
  tourner(-90);
  delay(50);

  Avancer(43.5);
  delay(50);

  tourner(--45);
  delay(50);

  Avancer(66);
  delay(50);

  pince(1);//fermer les pinces
  delay(50);

  tourner(180);
  delay(50);

  Avancer(111);
  delay(50);

  pince(0);//ouvrir les pinces
  delay(50);

  tourner(-135);
  delay(50);

  avancer(100);
  delay(50);
}

void fonctionvert()
{
  tourner(90);
  delay(50);

  Avancer(43.5);
  delay(50);

  tourner(-90);
  delay(50);

  Avancer(86.5);
  delay(50);

  tourner(45);
  delay(50);

  Avancer(66);
  delay(50);

  pince(1);
  delay(50);

  tourner(180);
  delay(50);

  avancer(111);
  delay(50);

  pince(0);
  delay(50);

  tourner(-135);
  delay(50);

  avancer(100);
  delay(50);
}

void fonctionrouge()
{
  tourner(-90);
  delay(50);

  Avancer(43.5);
  delay(50);

  tourner(90);
  delay(50);

  Avancer(86.5);
  delay(50);

  tourner(-45);
  delay(50);

  Avancer(66);
  delay(50);

  pince(1);
  delay(50);

  tourner(180);
  delay(50);

  avancer(111);
  delay(50);

  pince(0);
  delay(50);

  tourner(135);
  delay(50);

  avancer(100);
  delay(50);
}*/
float PID(int pulseM, int pulseE, float vitesseM, long pulseT, long totalE)
{
  int erreurP = 0;
  int erreurI = 0;
  // Robot 30A    kP : .001   kI : .002
  // Robot 30B    kP : .002   kI : .002
  float kP = 0.001, kI = 0.002;

  erreurP = pulseM - pulseE;
  erreurI = pulseT - totalE;

  //Aide à déboguer
  Serial.print(pulseM);
  Serial.print("    ");
  Serial.print(pulseE);
  Serial.print("    ");
  Serial.println(erreurP);

  return (vitesseM + (erreurP * kP) + (erreurI * kI) );
}



void Avancer(float distance)
{
  int pulseM = 0, pulseE = 0;
  float vitesseM = 0, vitesseE = 0;
  float circonference = 23.938936;//cm
  long pulseT = 0, pulseCalculer = 0,  totalE = 0;
   
  
  ENCODER_ReadReset(1);
  ENCODER_ReadReset(0);

  //Débogage
  Serial.println("1");

  pulseCalculer = distance/circonference*3200;

  Serial.println(pulseCalculer);

  while (pulseT < pulseCalculer)
  {
    //Code pour accélérer
    if (vitesseM == 0 && vitesseE == 0)
    {
      for (float i = 0; i < 0.5; i += 0.01)//Valeur de vitesse a ajuster, mais pas oublier d'ajuster le delay en fonction
      {
        //Serial.println(i);
        vitesseM = i;

        pulseM = ENCODER_ReadReset(1);
        pulseE = ENCODER_ReadReset(0);

        totalE += pulseE;
        pulseT += pulseM;

        Serial.println(pulseT);

        vitesseE = PID(pulseM, pulseE, vitesseM, pulseT, totalE);
        
        MOTOR_SetSpeed(1, vitesseM);
        MOTOR_SetSpeed(0, vitesseE);

        //Cas ou la distance a parcourir est plus petite que la distance d'accélération
        if (pulseT >= pulseCalculer)
        {
          Serial.println("5");
          vitesseM = 0;
          vitesseE = vitesseM;
          MOTOR_SetSpeed(1, vitesseM);
          MOTOR_SetSpeed(0, vitesseE);
          break;
        }

        delay(20); //a checker la valeur
      }
      //Fin du code pour accélérer
    }
    else
    {
      delay(20);
      
      pulseM = ENCODER_ReadReset(1);
      pulseE = ENCODER_ReadReset(0);

      totalE += pulseE;
      pulseT += pulseM;
      Serial.println(pulseT);

      //Code pour ralentir
      if (pulseT >= pulseCalculer - 3200 && pulseCalculer > 5500)
      {
        for (float i = vitesseM; i >= 0.20; i -= 0.01)
        {
          vitesseM = i;
          pulseM = ENCODER_ReadReset(1);
          pulseE = ENCODER_ReadReset(0);

          totalE += pulseE;
          pulseT += pulseM;
          Serial.println(pulseT);
          vitesseE = PID(pulseM, pulseE, vitesseM, pulseT, totalE);
          MOTOR_SetSpeed(1, vitesseM);
          MOTOR_SetSpeed(0, vitesseE);

          delay(20); //a checker la valeur
        }
      }
      //Fin du code pour ralentir

      //Code lorsque le robot roule a vitesse constante
      Serial.println("Allo");


      vitesseE = PID(pulseM, pulseE, vitesseM, pulseT, totalE);
      MOTOR_SetSpeed(0, vitesseE);
    }
  }
  MOTOR_SetSpeed(1, 0);
  MOTOR_SetSpeed(0, 0);

  delay(100);//Pour éviter de l'écrire après chaque fonctions

}


void Tourner(int32_t angle)
{
  int32_t nbPulse = 0, pulseM = 0, pulseE = 0;
  long compteurPulseG = 0, compteurPulseD = 0;
  float circonference = 23.938936; //Diamètre des roues en cm * Pi
  float arc; // Pi*d*angle/360   //d= 2*19.05 cm
  float arcUnitaire =  PI * 18.385 * 2 / 360;//arc pour un degré de rotation
  float vitesseG = 0.25, vitesseD = 0.25; // vitesse des moteurs

  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);

  arc = arcUnitaire * (angle / 2);
  nbPulse = arc/circonference * 3200;
  Serial.println(nbPulse);
  

  if (angle > 0)
  {
    MOTOR_SetSpeed(0,vitesseG);
    MOTOR_SetSpeed(1,-vitesseD);

    while(compteurPulseD > -nbPulse){
      delay(25);
      pulseM = ENCODER_ReadReset(1);
      pulseE = ENCODER_ReadReset(0);

      compteurPulseD += pulseM;
      compteurPulseG += pulseE;

      vitesseG = PID(pulseM, pulseE, vitesseD, -compteurPulseD, compteurPulseG); //le negative est la parce que les roues ne tournent pas dans la même direction, ce que le pid est normalement fait pour

      MOTOR_SetSpeed(0, vitesseG);

      Serial.print(compteurPulseD);
      Serial.print("    ");
      Serial.print(nbPulse);
      Serial.print("    ");
      Serial.println(compteurPulseG);

      //On pourrait pt juste attendre qu'il sorte du while puis mettre les vitesses à 0
      if (compteurPulseD <= -nbPulse){
        MOTOR_SetSpeed(1,0);
        MOTOR_SetSpeed(0,0);
      }
    }
  }
  else
  {
    Serial.println("Allo");
    MOTOR_SetSpeed(1,vitesseD);
    MOTOR_SetSpeed(0,-vitesseG);
    

    while(compteurPulseG > nbPulse){
      delay(25);
      pulseM = ENCODER_ReadReset(0);
      pulseE = ENCODER_ReadReset(1);

      compteurPulseG += pulseM;
      compteurPulseD += pulseE;

      vitesseD = PID(pulseM, pulseE, vitesseG, -compteurPulseD, compteurPulseG);

      MOTOR_SetSpeed(1, vitesseD);

      Serial.print(compteurPulseD);
      Serial.print("    ");
      Serial.print(nbPulse);
      Serial.print("    ");
      Serial.println(compteurPulseG);


      //On pourrait pt juste attendre qu'il sorte du while puis mettre les vitesses à 0
      if (compteurPulseD >= -nbPulse){
        MOTOR_SetSpeed(1,0);
        MOTOR_SetSpeed(0,0);
      }
    }
    MOTOR_SetSpeed(1,0);
        MOTOR_SetSpeed(0,0);
  }
  /*else
  {
    MOTOR_SetSpeed(0,-0.25);
    MOTOR_SetSpeed(1,0.25);

    while(-compteurPulseG > nbPulse){
      delay(25);
      pulseM = ENCODER_ReadReset(0);
      pulseE = ENCODER_ReadReset(1);

      compteurPulseG += pulseM;
      compteurPulseD += pulseE;

      vitesseD = PID(pulseE, pulseM, vitesseG, -compteurPulseG, compteurPulseD); //le negative est la parce que les roues ne tournent pas dans la même direction, ce que le pid est normalement fait pour

      MOTOR_SetSpeed(1, vitesseD);

      Serial.print(compteurPulseD);
      Serial.print("    ");
      Serial.print(nbPulse);
      Serial.print("    ");
      Serial.println(compteurPulseG);

      //On pourrait pt juste attendre qu'il sorte du while puis mettre les vitesses à 0
      if (compteurPulseG <= -nbPulse){
        MOTOR_SetSpeed(1,0);
        MOTOR_SetSpeed(0,0);
      }
    }
  }*/
  
  delay(100);//Pour éviter de l'écrire après chaque fonctions
}