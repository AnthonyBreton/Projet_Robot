/*
Projet: Défi du parcours - Manette IR
Equipe: P-30
Auteurs: Félix Richard
Description: Code permettant de contrôler le robot avec une manette IR pour le défi du parcours
Date: 2019/11/03
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */
#include <Arduino.h>
#include <LibRobus.h> // Essentielle pour utiliser RobUS



/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces

//tous les codes utilisés provenant de la manette IR
#define IR_Avancer          16736925  //0x00ff629d
#define IR_Reculer          16754775  //0x00ffa857
#define IR_Droite           16761405  //0x00ffc23d
#define IR_Gauche           16720605  //0x00ff22dd
#define IR_BalleRamasser    16769055  //0x00ff30cf
#define IR_BalleDeposer     16748655  //0x00ff18e7
#define IR_Stop             16716015  //0x00ff02fd
#define IR_Vitesse1         16724175  //0x00ff6897
#define IR_Vitesse2         16718055  //0x00ff9867
#define IR_Vitesse3         16743045  //0x00ffb04f

//Angles minimal et maximal du servo pour la pince
#define angle2 50
#define angle1 160

float vitesse = 0.3;
/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void BalleRamasser()
{
  SERVO_SetAngle(1,angle1);

}

void BalleDeposer()
{
  SERVO_SetAngle(1,angle2);

}

float PID(int pulseM, int pulseE, float vitesseM, long pulseT, long totalE)
{
  int erreurP = 0;
  int erreurI = 0;
  // Robot 30A    kP : .001   kI : .002
  // Robot 30B    kP : .002   kI : .002
  float kP = 0.002, kI = 0.002;

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

void Avancer(float vitesse)
{
int32_t nbPulse = 0, pulseM = 0, pulseE = 0;
  long compteurPulseG = 0, compteurPulseD = 0;
  //float circonference = 23.938936; //Diamètre des roues en cm * Pi
  //float arc; // Pi*d*angle/360   //d= 2*19.05 cm
  ////float arcUnitaire =  PI * 18.385 * 2 / 360;//arc pour un degré de rotation
  float vitesseG = 0.25, vitesseD = 0.25; // vitesse des moteurs
  int i=0;
  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);

  MOTOR_SetSpeed(0,vitesse);
  MOTOR_SetSpeed(1,vitesse);

  while(i=0)
    {
  delay(25);
        pulseM = ENCODER_ReadReset(1);
        pulseE = ENCODER_ReadReset(0);

        compteurPulseD += pulseM;
        compteurPulseG += pulseE;

        vitesseG = PID(pulseM, pulseE, vitesseD, compteurPulseD, compteurPulseG); //le negative est la parce que les roues ne tournent pas dans la même direction, ce que le pid est normalement fait pour

        MOTOR_SetSpeed(0, vitesseG);
  }



}


/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup(){
  BoardInit();
  SERVO_Enable(1);
  SERVO_SetAngle(1,angle2);
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop() {

  uint32_t code = REMOTE_read();

  switch (code)
  {
  case IR_Avancer:
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
    delay(100);

    Avancer(vitesse);
    break;

  case IR_Reculer:
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
    delay(100);

    Avancer(-vitesse);
    break;
  
  case IR_Droite:
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
    delay(100);

    MOTOR_SetSpeed(0,vitesse/2);
    MOTOR_SetSpeed(1,-vitesse/2);
    break;
  
  case IR_Gauche:
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
    delay(100);

    MOTOR_SetSpeed(0,-vitesse/2);
    MOTOR_SetSpeed(1,vitesse/2);
    break;
  
  case IR_BalleRamasser:
    BalleRamasser();
    break;

  case IR_BalleDeposer:
    BalleDeposer();
    break;
  
  case IR_Stop:
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
    break;
  
  case IR_Vitesse1:
    vitesse = 0.2;
    break;

  case IR_Vitesse2:
    vitesse = 0.3;
    break;

  case IR_Vitesse3:
    vitesse = 0.5;
    break;
  

  default:
    break;
  }
/*
  switch (code)
  {
  case IR_Avancer:
    Avancer(vitesse);
    break;

  case IR_Reculer:
    Reculer(vitesse);
    break;
  
  case IR_Droite:
    Tourner(vitesse);
    break;
  
  case IR_Gauche:
    Tourner(-vitesse);
    break;
  
  case IR_BalleRamasser:
    BalleRamasser();
    break;

  case IR_BalleDeposer:
    BalleDeposer();
    break;
  
  case IR_Stop:
    MOTOR_SetSpeed(1,0);
    MOTOR_SetSpeed(0,0);
    break;
  
  case IR_Vitesse1:
    vitesse = 0.2;
    break;

  case IR_Vitesse2:
    vitesse = 0.3;
    break;

  case IR_Vitesse3:
    vitesse = 0.5;
    break;
  

  default:
    break;
  }
  */
  delay(10);// Delais pour décharger le CPU
}