/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

#include <Arduino.h>
#include <librobus.h>
#define PI 3.14159265359


void Avancer(float distance);
void Tourner(int32_t angle);
float PID(int32_t nbPulseMaitre, int32_t nbPulseEsclave, float vEsclave, int nbIterations, int erreurTotal, int32_t nbPulseAttendu);

void setup() 
{
  BoardInit();

 // Tourner(360);
  //delay(1000);
  //Tourner(-360);
  //Serial.begin(9600);
 

  Avancer(120);
  delay(100);

  Tourner(-90);
  delay(100);

  Avancer(71);
  delay(100);

  Tourner(90);
  delay(100);

  Avancer(78);
  delay(100);

  Tourner(44); //45
  delay(100);

  Avancer(180);
  delay(100);
  
  Tourner(-89);
  delay(100);

  Avancer(50);
  delay(100);

  Tourner(46);
  delay(100);

  Avancer(100); 
  //UTURN DAWG
  UTurn();
  //UTURN DAWG
 Avancer(100); 
  delay(100);

  Tourner(-45);
  delay(100);

  Avancer(50);
  delay(100);

  Tourner(90);
  delay(100);

  Avancer(180);
  delay(100);

  Tourner(-44);
  delay(100);

  Avancer(78);
  delay(100);

  Tourner(-90);
  delay(100);

  Avancer(71);
  delay(100);

  Tourner(90);
  delay(100);

  Avancer(120);
  delay(100);

  Avancer(10); 
  UTurn();
  delay(10);
  UTurn();
  delay(10);
  UTurn();
  delay(10);
  UTurn();
  delay(10);

}



void loop() 
{
  
  
}


void Avancer(float distance)
{
    float vM=0.4;
    float vE=vM;
    int compteur = 0; //nb de pulse lu depuis de le debut
    int32_t nbPulse = 0, nbPulseM = 0, nbPulseE = 0; //M=maitre, E=esclave
    float circonference = 23.938936; //Diamètre des roues en cm * Pi
    int erreurL = 0, erreurT=0; //erreurL=erreur local, erreurT=erreur total
    float KP=0.0001 , KI=0.00002;//coefficiant pour erreurL et erreurT
    //2019-10-02 avant changement: KP=0.0001, KI=0.0002
    //Trouver le nombre de pulse nécessaire pour une distance donnée en cm
    nbPulse = distance / circonference * 3200;

    //Réinitialise les compteurs de pulses
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    //Initialiser la vitesse des deux moteurs à 50%
    MOTOR_SetSpeed(0, vM);
    MOTOR_SetSpeed(1, vE);

    //Permet de réduire la vitesse juste avant la fin pour éviter un arrêt brusque
    while (compteur < (nbPulse-3200))
    {
        delay(100);
        nbPulseM = ENCODER_Read(0);
        nbPulseE = ENCODER_Read(1);

        //on trouve les erreur L et T
        erreurL= nbPulseM-nbPulseE;
        compteur+= nbPulseM; //SI sa fonctionne pas, on fait la moyenne des 2 pulse (M et E)
        erreurT += erreurL;
        //calcul de la nouvelle vitesse pour vE
        vE= vM + erreurL * KP + (erreurT) * KI;
        MOTOR_SetSpeed(1,vE);

        //On reset les lectures des pulses
        ENCODER_ReadReset(0);
        ENCODER_ReadReset(1);
    }

    //Réduit la vitesse des moteurs
    vM=0.2;
    vE=vM;
    MOTOR_SetSpeed(0, vM);
    MOTOR_SetSpeed(1, vE);

    //Détermine quand le robot à atteint la distance pour l'arrêter
    while (compteur < nbPulse)
    {
        delay(100);
        nbPulseM = ENCODER_Read(0);
        nbPulseE = ENCODER_Read(1);

        //on trouve les erreur L et T
        erreurL= nbPulseM-nbPulseE;
        compteur+= nbPulseM; //SI sa fonctionne pas, on fait la moyenne des 2 pulse (M et E)
        erreurT += erreurL;

        //calcul de la nouvelle vitesse pour vE
        vE= vE + erreurL * KP + (erreurT) * KI;
        MOTOR_SetSpeed(1,vE);

        //On reset les lectures des pulses
        ENCODER_ReadReset(0);
        ENCODER_ReadReset(1);
    }


    //Arrête les moteurs
    vE=vM=0;
    MOTOR_SetSpeed(0, vM);
    MOTOR_SetSpeed(1, vE);
}


void Tourner(int32_t angle){
  int32_t nbPulse=0,compteurPulse=0; // 
  float circonference = 23.938936; //Diamètre des roues en cm * Pi
  float arc; // Pi*d*angle/360   //d= 2*19.05 cm
  float arcUnitaire =  PI * 18.2 * 2 / 360;//arc pour un degré de rotation
  float vitesse = 0.3; // vitesse des moteurs


  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);
  
  arc = arcUnitaire*angle;
  nbPulse = arc/circonference * 3200;
  

  if (nbPulse > 0){
    MOTOR_SetSpeed(0,vitesse);

    while(compteurPulse < (nbPulse)){//+ 50 en test pcq tourne pas assez
      delay(15);
      compteurPulse = ENCODER_Read(0);
    }

    MOTOR_SetSpeed(0,0);
  }
  else{
     MOTOR_SetSpeed(1,vitesse);

    while(compteurPulse < (nbPulse*(-1))){
      delay(15);
      compteurPulse = ENCODER_Read(1);
    }

    MOTOR_SetSpeed(1,0);
  }
}

void UTurn(){
  int32_t nbPulse=0,compteurPulseG= 0, compteurPulseD=0; // 
  float circonference = 23.938936; //Diamètre des roues en cm * Pi
  float arc; // Pi*d*angle/360   //d= 2*19.05 cm
  float arcUnitaire =  PI * 18.7 * 2 / 360;//arc pour un degré de rotation
  float vitesse = 0.3; // vitesse des moteurs


  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);


  arc = arcUnitaire*88;
  nbPulse = arc/circonference * 3200;
Serial.println(nbPulse);
  MOTOR_SetSpeed(0,vitesse);
  MOTOR_SetSpeed(1,-vitesse);

  

  while(compteurPulseG < nbPulse || compteurPulseD > -nbPulse){
    delay(50);
    compteurPulseD = ENCODER_Read(1);
    compteurPulseG = ENCODER_Read(0);
   // Serial.println(compteurPulseD);
   // Serial.print(nbPulse);
  // Serial.println(compteurPulseG);
    if (compteurPulseG>=nbPulse){
      MOTOR_SetSpeed(0,0);
    
    } 
    if (compteurPulseD<=-nbPulse){
      MOTOR_SetSpeed(1,0);
    }
  }
}