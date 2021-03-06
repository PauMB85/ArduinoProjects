/***********************************************************
* Author: PauMB                                            *
* Program: turn on || turn of a led,                       *
*          with a button                                   *
* Created: 08/05/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/
#include <Console.h>

//ditial pins
int button = 2;
int led = 8;

//variables
int pushNow = 0;
int pushLast = 0;

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Console.begin();
  while(!Console);
  
  pinMode(button,INPUT);
  pinMode(led,OUTPUT);
  
  Console.println("Push the Button");

}

void loop() {
  // put your main code here, to run repeatedly:
  
  pushNow = digitalRead(button);

  if(pushNow == 1 && pushLast == 0){
    digitalWrite(led,HIGH);
    Console.println("Turn ON");
    pushLast = 1;
    pushNow = 0;
  }
  
  if(pushNow == 1 && pushLast == 1){
    digitalWrite(led,LOW);
    Console.println("Turn OFF");
    pushLast = 0;
    pushNow = 0;
  }
  
  delay(250);
}
