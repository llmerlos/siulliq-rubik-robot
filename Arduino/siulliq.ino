/*
 * Example using non-blocking mode to move until a switch is triggered.
 *
 * Copyright (C)2015-2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "DRV8825.h"
#include "SyncDriver.h"


/*
 * Variables comuns de cada motor
 */
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 180
// Microstepping mode. If you hardwired it to save pins, set to the same value here.
#define MICROSTEPS 32

/*
 * Connexions de cada motor
 */
//Motor L // E0
#define lEN 24
#define lDIR 28
#define lSTEP 26

//Motor R // E1
#define rEN 30
#define rDIR 34
#define rSTEP 36

//Motor F // X
#define fEN 38
#define fDIR A1
#define fSTEP A0

//Motor D // Y
#define dEN A2
#define dDIR A7
#define dSTEP A6

//Motor B // Z
#define bEN A8
#define bDIR 48
#define bSTEP 46

DRV8825 lStepper(MOTOR_STEPS, lDIR, lSTEP, lEN);
DRV8825 rStepper(MOTOR_STEPS, rDIR, rSTEP, rEN);
DRV8825 fStepper(MOTOR_STEPS, fDIR, fSTEP, fEN);
DRV8825 dStepper(MOTOR_STEPS, dDIR, dSTEP, dEN);
DRV8825 bStepper(MOTOR_STEPS, bDIR, bSTEP, bEN);

SyncDriver conRL(rStepper, lStepper);
SyncDriver conFB(fStepper, bStepper);
//SyncDriver conUD(uStepper, dStepper);

char m1;
char m2;
bool m2u;//moviment 2 utilitzat
int i=0;
int nmov = 0;

String sol;
int sollength;

bool disableMotor = true;


void setup() {
    //començar serial
    Serial.begin(9600);
    
    //Posar en marxa motors
    lStepper.begin(RPM, MICROSTEPS);
    rStepper.begin(RPM, MICROSTEPS);
    fStepper.begin(RPM, MICROSTEPS);
    dStepper.begin(RPM, MICROSTEPS);
    bStepper.begin(RPM, MICROSTEPS);

    lStepper.setEnableActiveState(LOW);
    rStepper.setEnableActiveState(LOW);
    fStepper.setEnableActiveState(LOW);
    dStepper.setEnableActiveState(LOW);
    bStepper.setEnableActiveState(LOW);

    delay(5000);

    Serial2.begin(9600); /* l'arduino obre el port serial amb Python configurat igual: a 9600 bps i 8 bits de dades, sense paritat */
    while (!Serial2) /* l'arduino espera que el port serial estigui disponible */
    {
      ;
    }
    while(sollength == 0){
      while (Serial2.available() > 0) {     //wait here until the data is available
        sol = Serial2.readString();
        sollength = sol.length();
        Serial.println(sol);
        Serial.println(sollength);
      }
    }
    m1 = sol.charAt(i);
    m2 = sol.charAt(i+1);
    m2u = false;
    
    delay(1000);

    lStepper.enable();
    rStepper.enable();
    fStepper.enable();
    dStepper.enable();
    bStepper.enable();

    delay(1000);
    

    /*
    Serial.begin(9600);
    while (!Serial);
    {
      ;
    }
    delay(1000);
    Serial.println("S1");
    while(Serial.available()<1){
      ;
    }
    m1 = Serial.read();
    //delay(200);
    Serial.println(m1);
    while(Serial.available()<1){
      ;
    }
    m2 = Serial.read();
    //delay(200);
    Serial.println(m2);
    
    m2u = false;

    Serial.println("S2");*/

    
}

void loop() {
  if(i < sollength){
  
  Serial.println(m1);
  Serial.println(m2);
  
  switch(m1){
    case 'f':
      switch(m2){
        case 'f':
          fStepper.rotate(180);
          m2u = true;
          break;
        case 'F':
          m2u = true;
          break;
        case 'b':
          conFB.rotate(90,90);
          m2u = true;
          break;
        case 'B':
          conFB.rotate(90,-90);
          m2u = true;
          break;
        default :
          fStepper.rotate(90);
          m1=m2;
      }
      break;
    case 'F':
      switch(m2){
        case 'F':
          fStepper.rotate(-180);
          m2u = true;
          break;
        case 'f':
          m2u = true;
          break;
        case 'b':
          conFB.rotate(-90,90);
          m2u = true;
          break;
        case 'B':
          conFB.rotate(-90,-90);
          m2u = true;
          break;
        default :
          fStepper.rotate(-90);
          m1=m2;
      }
      break;
    case 'b':
      switch(m2){
        case 'b':
          bStepper.rotate(180);
          m2u = true;
          break;
        case 'B':
          m2u = true;
          break;
        case 'f':
          conFB.rotate(90,90);
          m2u = true;
          break;
        case 'F':
          conFB.rotate(-90,90);
          m2u = true;
          break;
        default :
          bStepper.rotate(90);
          m1=m2;
      }
      break;
    case 'B':
      switch(m2){
        case 'B':
          fStepper.rotate(-180);
          m2u = true;
          break;
        case 'b':
          m2u = true;
          break;
        case 'f':
          conFB.rotate(90,-90);
          m2u = true;
          break;
        case 'F':
          conFB.rotate(-90,-90);
          m2u = true;
          break;
        default :
          bStepper.rotate(-90);
          m1=m2;
      }
      break;
    case 'r':
      switch(m2){
        case 'r':
          rStepper.rotate(180);
          m2u = true;
          break;
        case 'R':
          m2u = true;
          break;
        case 'l':
          conRL.rotate(90,90);
          m2u = true;
          break;
        case 'L':
          conRL.rotate(90,-90);
          m2u = true;
          break;
        default :
          rStepper.rotate(90);
          m1=m2;
      }
      break;
    case 'R':
      switch(m2){
        case 'R':
          rStepper.rotate(-180);
          m2u = true;
          break;
        case 'r':
          m2u = true;
          break;
        case 'l':
          conRL.rotate(-90,90);
          m2u = true;
          break;
        case 'L':
          conRL.rotate(-90,-90);
          m2u = true;
          break;
        default :
          rStepper.rotate(-90);
          m1=m2;
      }
      break;
    case 'l':
      switch(m2){
        case 'l':
          lStepper.rotate(180);
          m2u = true;
          break;
        case 'L':
          m2u = true;
          break;
        case 'r':
          conRL.rotate(90,90);
          m2u = true;
          break;
        case 'R':
          conRL.rotate(-90,90);
          m2u = true;
          break;
        default :
          lStepper.rotate(90);
          m1=m2;
      }
      break;
    case 'L':
      switch(m2){
        case 'L':
          lStepper.rotate(-180);
          m2u = true;
          break;
        case 'l':
          m2u = true;
          break;
        case 'r':
          conRL.rotate(90,-90);
          m2u = true;
          break;
        case 'R':
          conRL.rotate(-90,-90);
          m2u = true;
          break;
        default :
          lStepper.rotate(-90);
          m1=m2;
      }
      break;
    case 'd':
      switch(m2){
        case 'd':
          dStepper.rotate(180);
          m2u = true;
          break;
        case 'D':
          m2u = true;
          break;
        default :
          dStepper.rotate(90);
          m1=m2;
      }
      break;
    case 'D':
      switch(m2){
        case 'D':
          dStepper.rotate(-180);
          m2u = true;
          break;
        case 'd':
          m2u = true;
          break;
        default :
          dStepper.rotate(-90);
          m1=m2;
      }
      break;
  }

  if(m2u){
    i++;
    i++;
    m1 = sol.charAt(i);
    m2 = sol.charAt(i+1);
  } else {
    i++;
    m1 = sol.charAt(i);
    m2 = sol.charAt(i+1);
  }
  m2u=false;
  
} else if (disableMotor){
    disableMotor = false;
    lStepper.disable();
    rStepper.disable();
    fStepper.disable();
    dStepper.disable();
    bStepper.disable();
}
}

/*if(m2u){
    while(Serial.available()<1){
      ;
    }
    m1 = Serial.read();
    while(Serial.available()<1){
      ;
    }
    m2 = Serial.read();  
  } else {
    while(Serial.available()<1){
      ;
    }
    m2 = Serial.read(); 
  }
  m2u=false;
*/
/*
    conRL.rotate(90*5, -90);
    delay(1000);
    conRL.rotate(-90, -90);
    delay(1000);
    lStepper.rotate(90);
    delay(5000);
    conFB.rotate(90*5, -90);
    delay(1000);
    conFB.rotate(-90, -90);
    delay(1000);
    conFB.rotate(0, 90);    
    delay(5000);
    dStepper.rotate(90);
    delay(30000);
}*/
