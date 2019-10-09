/*
30A
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
  delay(1000);
/*
  Tourner(-90);
  delay(1000);
  Tourner(90);
  delay(1000);
  Tourner(-180);
  delay(1000);
  Tourner(180);
  delay(1000);
*/

  Avancer(116);
  delay(100);

  Tourner(-90);
  delay(100);

  Avancer(71);
  delay(100);

  Tourner(90);
  delay(100);

  Avancer(78);
  delay(100);

  Tourner(45);
  delay(100);

  Avancer(172);
  delay(100);
  
  Tourner(-90);
  delay(100);

  Avancer(53);
  delay(100);

  Tourner(45);
  delay(100);

  Avancer(108); 
  delay(100);

  UTurn();
  delay(100);
 
  Avancer(110); 
  delay(100);

  Tourner(-45);
  delay(100);

  Avancer(50);
  delay(100);

  Tourner(91);
  delay(100);

  Avancer(172);
  delay(100);

  Tourner(-45);
  delay(100);

   Avancer(78);
  delay(100);

  Tourner(-90);
  delay(100);

  Avancer(75);
  delay(100);

  Tourner(90);
  delay(100);

  Avancer(116);
  delay(100);

  UTurn();UTurn();UTurn();UTurn();UTurn();
  
}



void loop() 
{
  
  
}


void Avancer(float distance)
{
    float vM=0.2;
    float vE=vM;
    int compteurM = 0, compteurE = 0; //nb de pulse lu depuis de le debut
    int32_t nbPulse = 0, nbPulseM = 0, nbPulseE = 0; //M=maitre, E=esclave
    float circonference = 23.938936; //Diamètre des roues en cm * Pi
    int erreurL = 0, erreurT=0; //erreurL=erreur local, erreurT=erreur total
    float KP=0.0006 , KI=0.00002;//coefficiant pour erreurL et erreurT
    //float KP=0.0004 , KI=0.00002; avant changement 2019/10/09
    //2019-10-02 avant changement: KP=0.0001, KI=0.00002
    //Trouver le nombre de pulse nécessaire pour une distance donnée en cm
    nbPulse = distance / circonference * 3200;

    //Réinitialise les compteurs de pulses
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    //Initialiser la vitesse des deux moteurs à 50%
    MOTOR_SetSpeed(0, vM);
    MOTOR_SetSpeed(1, vE);

    while(vM <= 0.5){
      vM += 0.01;
      vE = vM;
      MOTOR_SetSpeed(0, vM);
      MOTOR_SetSpeed(1, vE);
      delay(15);
    }

    //Permet de réduire la vitesse juste avant la fin pour éviter un arrêt brusque
    while (compteurM < (nbPulse-3200))
    {
        delay(100);
        nbPulseM = ENCODER_Read(0);
        nbPulseE = ENCODER_Read(1);

        //on trouve les erreur L et T
        erreurL= nbPulseM-nbPulseE;
        compteurM+= nbPulseM; 
        compteurE+= nbPulseE; 
        erreurT =compteurM-compteurE;
        //calcul de la nouvelle vitesse pour vE
        vE= vE + erreurL * KP + erreurT * KI;
        MOTOR_SetSpeed(1,vE);

        //On reset les lectures des pulses
        ENCODER_ReadReset(0);
        ENCODER_ReadReset(1);
    }

    //Réduit la vitesse des moteurs
    while(vM >= 0.2){
      vM -= 0.01;
      vE = vM;
      MOTOR_SetSpeed(0, vM);
      MOTOR_SetSpeed(1, vE);
      delay(10);
    }

    //Détermine quand le robot à atteint la distance pour l'arrêter
    while (compteurM < nbPulse)
    {
        delay(100);
        nbPulseM = ENCODER_Read(0);
        nbPulseE = ENCODER_Read(1);

        //on trouve les erreur L et T
        erreurL= nbPulseM-nbPulseE;
        compteurM+= nbPulseM; //SI sa fonctionne pas, on fait la moyenne des 2 pulse (M et E)
        compteurE+= nbPulseE;
        erreurT = compteurM - compteurE;

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

/*float PID(int32_t nbPulseMaitre, int32_t nbPulseEsclave, float vEsclave, int nbIterations, int erreurTotal, int32_t nbPulseAttendu)
{
    int erreurVitesse= nbPulseMaitre-nbPulseEsclave;
    int erreurIntegrale=(nbIterations*nbPulseAttendu)-erreurTotal;
    float correctionVitesse=(0.00005*erreurIntegrale+0.0001*erreurVitesse);
    vEsclave=vEsclave+correctionVitesse;
}

void Avancer(float distance)
{
    float vMaitre=0.9;
    float vEsclave=0.9;
    int nbIterations = 0, erreurTotal = 0;
    int32_t nbPulse = 0, nbPulseMaitre = 0, nbPulseEsclave = 0, nbPulseAttendu = 2500;
    float circonference = 23.938936; //Diamètre des roues en cm * Pi

    //Trouver le nombre de pulse nécessaire pour une distance donnée en cm
    nbPulse = distance / circonference * 3200;
    
    //Réinitialise les compteurs de pulses
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    //Initialiser la vitesse des deux moteurs à 50%
    MOTOR_SetSpeed(0, vMaitre);
    MOTOR_SetSpeed(1, vEsclave);

    //Permet de réduire la vitesse juste avant la fin pour éviter un arrêt brusque
    while (nbPulseMaitre < (nbPulse-3200))
    {
        nbPulseMaitre = ENCODER_Read(0);
        nbPulseEsclave = ENCODER_Read(1);
        erreurTotal += nbPulseEsclave;
        nbIterations++;
        MOTOR_SetSpeed(1, PID(nbPulseMaitre, nbPulseEsclave, vEsclave, nbIterations, erreurTotal, nbPulseAttendu));
        delay(100);
    }

    //Réduit la vitesse des moteurs
    vMaitre=0.2;
    vEsclave=vMaitre;
    MOTOR_SetSpeed(0, vMaitre);
    MOTOR_SetSpeed(1, vEsclave);
    nbPulseAttendu = 1000;

    //Détermine quand le robot à atteint la distance pour l'arrêter
    while (nbPulseMaitre < nbPulse)
    {
        nbPulseMaitre = ENCODER_Read(0);
        nbPulseEsclave = ENCODER_Read(1);
        erreurTotal += nbPulseEsclave;
        nbIterations++;
        MOTOR_SetSpeed(1, PID(nbPulseMaitre, nbPulseEsclave, vEsclave, nbIterations, erreurTotal, nbPulseAttendu));
        delay(100);
    }

    //Arrête les moteurs
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}*/


void Tourner(int32_t angle){
  int32_t nbPulse=0,compteurPulse=0; // 
  float circonference = 23.938936; //Diamètre des roues en cm * Pi
  float arc; // Pi*d*angle/360   //d= 2*19.05 cm
  float arcUnitaire =  PI * 18.75 * 2 / 360;//arc pour un degré de rotation
  float vitesse = 0.3; // vitesse des moteurs


  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);


  arc = arcUnitaire*angle;
  nbPulse = arc/circonference * 3200;

  if (nbPulse > 0){
    MOTOR_SetSpeed(0,vitesse);

    while(compteurPulse < (nbPulse)){//+ 50 en test pcq tourne pas assez

      delay(50);

      compteurPulse = ENCODER_Read(0);
    }

    MOTOR_SetSpeed(0,0);
  }
  else{
     MOTOR_SetSpeed(1,vitesse);

    while(compteurPulse < (nbPulse*(-1))){

      delay(50);

      compteurPulse = ENCODER_Read(1);
    }

    MOTOR_SetSpeed(1,0);
  }
}



void UTurn(){
  float vM=0.3;
  float vE=-vM;
  int compteur = 0; //nb de pulse lu depuis de le debut
  int32_t nbPulse = 0, nbPulseM = 0, nbPulseE = 0; //M=maitre, E=esclave
  float circonference = 23.938936; //Diamètre des roues en cm * Pi
  int erreurL = 0, erreurT=0; //erreurL=erreur local, erreurT=erreur total
  float KP=0.0002 , KI=0.00002;//coefficiant pour erreurL et erreurT<
  float arc; // Pi*d*angle/360   //d= 2*19.05 cm
  float arcUnitaire =  PI * 19.7 * 2 / 360;//arc pour un degré de rotation
  //2019-10-02 avant changement: KP=0.0001, KI=0.00002
  //Trouver le nombre de pulse nécessaire pour une distance donnée en cm
  arc = arcUnitaire*90;
  nbPulse = arc/circonference * 3200;
  //Serial.println(nbPulse);

  //Réinitialise les compteurs de pulses
  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);

  //Initialiser la vitesse des deux moteurs à 50%
  MOTOR_SetSpeed(0, vM);
  MOTOR_SetSpeed(1, vE);

  while(nbPulseM < nbPulse){
    delay(10);
    nbPulseE = ENCODER_Read(1);
    nbPulseM = ENCODER_Read(0);

    //on trouve les erreur L et T
    erreurL= nbPulseM-(-nbPulseE);
    compteur+= nbPulseM; //SI sa fonctionne pas, on fait la moyenne des 2 pulse (M et E)
    erreurT += erreurL;
    //calcul de la nouvelle vitesse pour vE
    vE= vM - erreurL * KP - (erreurT) * KI;

    if (nbPulseM>=nbPulse){
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);
    }
  }
}
