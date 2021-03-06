#include "EngineMindPID.h"
#include "Utils.h"
#include <WProgram.h>

void EngineMind::start(){  
  
  Serial.println("\tINITILIZING ENGINE PIN");
  motori[0].attach( motore1PWMPin, 1000, 2400 );
  motori[1].attach( motore2PWMPin, 1000, 2400 );
  motori[2].attach( motore3PWMPin, 1000, 2400 );
  motori[3].attach( motore4PWMPin, 1000, 2400 );
  
  Serial.println("\tINITILIZING PID");
  pidMotori[0].setP(0);
  pidMotori[0].setI(0);
  pidMotori[0].setD(0);
  
  pidMotori[1].setP(0);
  pidMotori[1].setI(0);
  pidMotori[1].setD(0);
  
  Serial.println("\tINITILIZING ESC");
  int i;
  for (i=0; i < 4; i++){
    motori[i].write( 0 );
  }
}
float enginesPowerFloat=0;
float pid0=0;
float pid1=0;

void EngineMind::update(float engineAngle[], float desiredAngle[], int enginesPower){
  //float enginesPowerFloat = enginesPower*0.004;//so get enginepowerfloat * 1000 = from 400 to 0 if engine power is from 0 to 100
  /*
  Serial.println("debug");
  
  Serial.print(engineAngle[0]);
  Serial.print(" ");
  Serial.print(engineAngle[1]);
  Serial.print(" ");
  Serial.println(engineAngle[2]);
  */
  enginesPowerFloat = Utils::mapFloat (enginesPower, 0, 100, MIN_ENGINE, MAX_ENGINE);
  /*
  Serial.print(enginesPowerFloat);
  Serial.print(" ");
  Serial.print(MIN_ENGINE);
  Serial.print(" ");
  Serial.println(MAX_ENGINE);
  */
  pid0=pidMotori[0].update(engineAngle[2], desiredAngle[2]);
  //Serial.println(pid0);
  valoreReale[0]=enginesPowerFloat+pid0;
  valoreReale[1]=enginesPowerFloat-pid0;
  /*
  //test if the power is over the max
  if (valoreReale[0] > 100){
    valoreReale[1]-=valoreReale[0];
    valoreReale[0]=MAX_ENGINE;
  }
  if (valoreReale[1] > MAX_ENGINE){
    valoreReale[0]-=valoreReale[1];
    valoreReale[1]=MAX_ENGINE;
  }
  
  //test if the power is less the max
  if (valoreReale[0] < MIN_ENGINE){
    valoreReale[1]+=abs(valoreReale[0]);
    valoreReale[0]=MIN_ENGINE;
  }
  if (valoreReale[1] < MIN_ENGINE){
    valoreReale[0]+=abs(valoreReale[1]);
    valoreReale[1]=MIN_ENGINE;
  }
  //here we can have value over max power, but it's not a problem
  */
  
  
  pid1=pidMotori[1].update(engineAngle[1], desiredAngle[1]);
  //Serial.println(pid1);
  valoreReale[2]=enginesPowerFloat+pid1;
  valoreReale[3]=enginesPowerFloat-pid1;
  /*
  //test if the power is over the max
  if (valoreReale[2] > MAX_ENGINE){
    valoreReale[3]-=valoreReale[2];
    valoreReale[2]=MAX_ENGINE;
  }
  if (valoreReale[3] > MAX_ENGINE){
    valoreReale[2]-=valoreReale[3];
    valoreReale[3]=MAX_ENGINE;
  }
  
  //test if the power is less the max
  if (valoreReale[2] < MIN_ENGINE){
    valoreReale[3]+=abs(valoreReale[2]);
    valoreReale[2]=MIN_ENGINE;
  }
  if (valoreReale[3] < MIN_ENGINE){
    valoreReale[2]+=abs(valoreReale[3]);
    valoreReale[3]=MIN_ENGINE;
  }
  */
  
  
  /*
   * SET THE POWER TO ENGINE!
   */
  for (int i=0;i<4;i++){
    valoreReale[i]= constrain(valoreReale[i], MIN_ENGINE, MAX_ENGINE);
    motori[i].write( (int)valoreReale[i] );
  }
}

void EngineMind::debug(){

  Serial.print("Valori reali:");
  for (int i=0;i<4;i++){
    Serial.print(" ");
    Serial.print(valoreReale[i]);
  }
  Serial.println();
  
  Serial.print("Valori PID:");
  Serial.print(" ");
  Serial.print( pidMotori[0].getP() );
  Serial.print(" ");
  Serial.print( pidMotori[0].getI() );
  Serial.print(" ");
  Serial.print( pidMotori[0].getD() );
  Serial.println();
  
  Serial.print("Valori PID0:");
  Serial.println(pid0);
  
  Serial.print("Valori PID1:");
  Serial.println(pid1);
  
  Serial.print("Valori enginesPowerFloat:");
  Serial.println(enginesPowerFloat);
  
  /*
  Serial.print("Valori PPM:");
  for (int i=0;i<4;i++){
    Serial.print(" ");
    Serial.print(valorePPM[i]);
  }
  Serial.println();
  */
  //Serial.println();
}

void EngineMind::brake(){
  int i;
  for (i=0; i < 4; i++){
    motori[i].write( 0 );
  }
}

void EngineMind::setP(float p){
  pidMotori[0].setP(p);
  pidMotori[1].setP(p);
}

void EngineMind::setI(float i){
  pidMotori[0].setI(i);
  pidMotori[1].setI(i);
}

void EngineMind::setD(float d){
  pidMotori[0].setD(d);
  pidMotori[1].setD(d);
}