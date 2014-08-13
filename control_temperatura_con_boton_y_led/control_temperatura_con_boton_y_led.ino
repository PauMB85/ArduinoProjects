
/***********************************************************
* Author: PauMB                                            *
* Program: turn on || turn of a led,                       *
*          in fuction of NTC sensor                        *
* Created: 08/02/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/

//libreries
#include <math.h>

//Digital Pin
int led = 13; //Led

//Analog Pin
int rNTC = 0; //rNTC (A0)

//Values of NTC
double vin = 5.0; //v. input
double r = 1000.0; //r who combine with ntc
double r25 = 2800.0; //r of NTC in 25gree
double beta = 3900.0; //constant beta in 25
double t0 = 293.15; //temperatue in kelvin (25)

//Values how calculete the RNTC
double vout = 0.0;
double R_NTC = 0.0;

//Temperature
double tK = 0.0; //temperature in Kelvin
double tC = 0.0; //temperature in C

void setup() {

  Serial.begin(9600);
  while(!Serial);
  pinMode(led,OUTPUT);
  pinMode(rNTC,INPUT);

}

void loop() {
  
  //Calculate the vout & R_NTC
  vout = vin/1024* analogRead(rNTC); //1024  A/D
  R_NTC = vout*r/(vin-vout);
  
  //Calculete the temperature
  tK= beta/(log(R_NTC/r25)+(beta/t0));
  tC = tK-273.15;
  
  //show in terminal
  Serial.print("La temperatura en K:");
  Serial.println(tK);
  Serial.print("La temperatura en C:");
  Serial.println(tC);
  //digitalWrite(led,HIGH);
  
  if(tC>23.0){
    digitalWrite(led,HIGH); //turn on the led if the temperature is between 2 values
  }
  else if (tC < 21){
    digitalWrite(led,LOW); // turn off the led if the condition is not met
  } 
  delay(2000);
}
