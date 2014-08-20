/***********************************************************
* Author: PauMB                                            *
* Program: Ultrasonic Sensor & NTC                         * 
* Created: 08/20/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/

//importing libreries
#include <Console.h>

//digital pins
int Trigger = 4;    //Pulse output
int Echo = 2;       //Pulse intput

//analog pin
int rNTC = 3;

//variables Ultrasonic sensor
float Distance = 0;    //D = V * T / 2
long Time = 0;
float sofSound = 0;

//varibales for NTC
double vin = 5.0;      //v. input
double r = 1000.0;     //r who combine with ntc
double r25 = 2800.0;   //r of NTC in 25gree
double beta = 3900.0;  //constant beta in 25
double t0 = 293.15;    //temperatue in kelvin (25)

//Values how calculete the RNTC
double vout = 0.0;
double R_NTC = 0.0;

//Temperature
double tK = 0.0; //temperature in Kelvin
double tC = 0.0; //temperature in C

void setup() {
  
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);     //Turn on the led 13, while initialize the sketch
  Bridge.begin();
  Console.begin();
  pinMode(Trigger,OUTPUT);
  pinMode(Echo,INPUT);
  pinMode(rNTC,INPUT);
  while(!Console);          //Wait open the console
  digitalWrite(13,LOW);     //Turn off the led
  
}

void loop() {
  //Calculate the temperature
  temperature();
  
  //Calculate the speed of sound
  speedSound();
  
  //Calculate the distance
  distanceObject();
  
  //show in terminal
  Console.print("La temperatura en K:"); //only for debug
  Console.println(tK);
  Console.print("La temperatura en C:");
  Console.println(tC);
  Console.print("The speed of sound is: ");
  Console.println(sofSound); //for debug, you can delete
  
  
  Console.print("distance to the object is: ");
  Console.print(Distance);
  Console.println("cm");
  
  delay(2000);
}

void temperature(){
  //Calculate the vout & R_NTC
  vout = vin/1024* analogRead(rNTC); //1024  A/D
  R_NTC = vout*r/(vin-vout);
  
  //Calculete the temperature in K & C
  tK= beta/(log(R_NTC/r25)+(beta/t0));
  tC = tK-273.15;
}

void speedSound(){
    
  sofSound = (331+0.6*tC)/1000000*100; //m/s --> cm/us

}

void distanceObject(){
  digitalWrite(Trigger,LOW);
  delay(20);  //wait 20ms for every cycle
  digitalWrite(Trigger,HIGH); //10us TTL
  delayMicroseconds(10); //wait 10us
  digitalWrite(Trigger,LOW);
  Time = pulseIn(Echo,HIGH); //input TTL, signal with a range in proportion (us)
  Distance = sofSound*Time;  //Calculate the distance in meters
}
