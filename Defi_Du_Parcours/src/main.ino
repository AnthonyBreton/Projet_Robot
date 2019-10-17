/*
Projet: Test de PID
Equipe: Ben et Tony
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

#include <Arduino.h>
#include <librobus.h>
#define PI 3.14159265359


void Avancer(float distance);
void Tourner(int32_t angle);
float PID(int pulseM, int pulseE, float vitesseM);

void setup() 
{
  BoardInit();
  Serial.begin(9600);

  Avancer(100);
}



void loop() 
{
  
  
}

float PID(int pulseM, int pulseE, float vitesseM)
{
  int erreurP = 0;
  float kP = 0.001;

  erreurP = pulseM - pulseE;

  Serial.print(pulseM);
  Serial.print("    ");
  Serial.print(pulseE);
  Serial.print("    ");
  Serial.println(erreurP);

  return (vitesseM + (erreurP * kP) );
}


void Avancer(float distance)
{
  int pulseCalculer = 0, pulseM = 0, pulseE = 0, pulseT = 0;
  float vitesseM = 0, vitesseE = 0;
  float circonference = 23.938936;//cm
  
  
  ENCODER_ReadReset(1);
  ENCODER_ReadReset(0);

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

        pulseM = ENCODER_Read(1);
        pulseE = ENCODER_Read(0);

        pulseT += pulseM;

        vitesseE = PID(pulseM, pulseE, vitesseM);
        
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

        ENCODER_ReadReset(1);
        ENCODER_ReadReset(0);
        delay(20); //a checker la valeur
      }
      //Fin du code pour accélérer
    }
    else
    {
      pulseM = ENCODER_Read(1);
      pulseE = ENCODER_Read(0);

      pulseT += pulseM;
      //Serial.println(pulseT);

      //Code pour ralentir
      if (pulseT >= pulseCalculer - 3200)
      {
        for (float i = vitesseM; i >= 0.20; i -= 0.01)
        {
          vitesseM = i;
          pulseM = ENCODER_Read(1);
          pulseE = ENCODER_Read(0);

          pulseT += pulseM;
          //Serial.println(pulseT);
          vitesseE = PID(pulseM, pulseE, vitesseM);
          MOTOR_SetSpeed(1, vitesseM);
          MOTOR_SetSpeed(0, vitesseE);

          ENCODER_ReadReset(1);
          ENCODER_ReadReset(0);
          delay(20); //a checker la valeur
        }
      }
      //Fin du code pour ralentir

      //Code lorsque le robot roule a vitesse constante

      vitesseE = PID(pulseM, pulseE, vitesseM);


      ENCODER_ReadReset(1);
      ENCODER_ReadReset(0);
      delay(10);
      
    }
  }
  MOTOR_SetSpeed(1, 0);
  MOTOR_SetSpeed(0, 0);

  delay(100);//Pour éviter de l'écrire après chaque fonctions

}



/*
void Avancer2(float distance)
{
    float vM=0.5;
    float vE=vM;
    int compteurM = 0, compteurE = 0; //nb de pulse lu depuis de le debut
    int32_t nbPulse = 0, nbPulseM = 0, nbPulseE = 0; //M=maitre, E=esclave
    float circonference = 23.938936; //Diamètre des roues en cm * Pi
    int erreurL = 0, erreurT=450; //erreurL=erreur local, erreurT=erreur total
    float KP=0.0007 , KI=0.00020;//coefficiant pour erreurL et erreurT
    //2019-10-09 avant changement: KP=0.0001, KI=0.00002
    //Trouver le nombre de pulse nécessaire pour une distance donnée en cm
    nbPulse = distance / circonference * 3200;

    //Réinitialise les compteurs de pulses
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    

    //Initialiser la vitesse des deux moteurs à 50%
    MOTOR_SetSpeed(0, vM);
    MOTOR_SetSpeed(1, vE);

    

    //Permet de réduire la vitesse juste avant la fin pour éviter un arrêt brusque
    
    while (compteurM < (nbPulse-3200))
    {
        delay(100);
        nbPulseM = ENCODER_Read(0);
        nbPulseE = ENCODER_Read(1);

        //on trouve les erreur L et T
        erreurL= nbPulseM-nbPulseE;
        //Serial.println(erreurL);
        compteurM += nbPulseM; //SI sa fonctionne pas, on fait la moyenne des 2 pulse (M et E)
        compteurE += nbPulseE;
        erreurT = compteurM-compteurE;
        Serial.print(erreurL);
        Serial.print("    ");
        Serial.println(erreurT);
        //calcul de la nouvelle vitesse pour vE
        vE= vM + erreurL * KP + erreurT * KI;
        MOTOR_SetSpeed(1,vE);

        //On reset les lectures des pulses
        ENCODER_ReadReset(0);
        ENCODER_ReadReset(1);
    }

    //Réduit la vitesse des moteurs
    while (vM >= 0.2)
    {
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
        compteurE += nbPulseE;
        erreurT += compteurM - compteurE;

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

void Avancer(float distance)
{
    float vM=0.2;
    float vE=vM;
    int compteurM = 0, compteurE = 0; //nb de pulse lu depuis de le debut
    int32_t nbPulse = 0, nbPulseM = 0, nbPulseE = 0; //M=maitre, E=esclave
    float circonference = 23.938936; //Diamètre des roues en cm * Pi
    int erreurL = 0, erreurT=450; //erreurL=erreur local, erreurT=erreur total
    float KP=0.0007 , KI=0.00020;//coefficiant pour erreurL et erreurT
    //2019-10-09 avant changement: KP=0.0001, KI=0.00002
    //Trouver le nombre de pulse nécessaire pour une distance donnée en cm
    nbPulse = distance / circonference * 3200;

    //Réinitialise les compteurs de pulses
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    

    //Initialiser la vitesse des deux moteurs à 50%
    MOTOR_SetSpeed(0, vM);
    MOTOR_SetSpeed(1, vE);

    while (vM <= 0.5)
    {
      vM += 0.01;
      vE = vM;
      MOTOR_SetSpeed(0, vM);
      MOTOR_SetSpeed(1, vE);
      delay(20);
    }

    //Permet de réduire la vitesse juste avant la fin pour éviter un arrêt brusque
    
    while (compteurM < (nbPulse-3200))
    {
        delay(100);
        nbPulseM = ENCODER_Read(0);
        nbPulseE = ENCODER_Read(1);

        //on trouve les erreur L et T
        erreurL= nbPulseM-nbPulseE;
        //Serial.println(erreurL);
        compteurM += nbPulseM; //SI sa fonctionne pas, on fait la moyenne des 2 pulse (M et E)
        compteurE += nbPulseE;
        erreurT = compteurM-compteurE;
        Serial.print(erreurL);
        Serial.print("    ");
        Serial.println(erreurT);
        //calcul de la nouvelle vitesse pour vE
        vE= vM + erreurL * KP + erreurT * KI;
        MOTOR_SetSpeed(1,vE);

        //On reset les lectures des pulses
        ENCODER_ReadReset(0);
        ENCODER_ReadReset(1);
    }

    //Réduit la vitesse des moteurs
    while (vM >= 0.2)
    {
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
        compteurE += nbPulseE;
        erreurT += compteurM - compteurE;

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
  float arcUnitaire =  PI * 18.75 * 2 / 360;//arc pour un degré de rotation
  float vitesse = 0.25; // vitesse des moteurs


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
  float arcUnitaire =  PI * 18.385 * 2 / 360;//arc pour un degré de rotation
  float vitesse = 0.2; // vitesse des moteurs


  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);


  arc = arcUnitaire*91;
  nbPulse = arc/circonference * 3200;
Serial.println(nbPulse);
  MOTOR_SetSpeed(0,vitesse);
  MOTOR_SetSpeed(1,-vitesse);

  

  while(compteurPulseG < nbPulse || compteurPulseD > -nbPulse){
    delay(15);
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
}*/