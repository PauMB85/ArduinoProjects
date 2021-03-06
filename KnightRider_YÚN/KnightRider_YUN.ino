/***********************************************************
* Author: PauMB                                            *
* Program: 3 knight rider effect,                          *
*          you choose it with console                      *
* Created: 08/06/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/
#include <Console.h>

//Buttons
int bRight = 7;
int bLeft = 2;

//Leds
int Leds [] = {4,8,12,13};

//variables
int pushRight = 0; //Detects if you push the right
int pushLeft = 0; //Detects if you push the left button
int right = 0;  //save right status
int left = 0;  // save left status

//for the console
char c;
char choose;

//for indicate the effect on time
int effect1 = 0;
int effect2 = 0;
int effect3 = 0;

void setup() {
 
  Bridge.begin();
  Console.begin();
  while(!Console);          //wait open Console
  pinMode(bRight,INPUT);    //star buttons
  pinMode(bLeft,INPUT);
  int i;                    //star leds
  for( i = 0; i < sizeof(Leds)/sizeof(int); i++)
  {
    pinMode(Leds[i],OUTPUT);
  }
  
  //print Menu
  Console.println("Choose the Knight Rider effect");
  Console.println("(1) Authomatic");
  Console.println("(2) One time");
  Console.println("(3) The same direction"); 

}

void loop() {
  
  //read console and save the option in choose
  if (Console.available() > 0) {

    c = Console.read();
    if( c != '\n'){
      choose = c;
    }
  }
  
  //in function of the 'choose'
  switch(choose){
    
    case '1': //1rt effect
      if(effect1 == 0){
        Console.println("Effect 1");
        effect1 = 1;
        effect2 = 0;
        effect3 = 0;
      }
      lightRight();
      lightLeft();
      break;
    case '2': //2nd effect
      if(effect2 == 0){
        Console.println("Effect 2");
        Console.println("push any button");
        effect1 = 0;
        effect2 = 1;
        effect3 = 0;
      }
      pushRight = digitalRead(bRight);
      pushLeft = digitalRead(bLeft);
      
      if(pushRight == 1){
        lightRight();
        lightLeft();
      }
      if(pushLeft == 1){
        lightLeft();
        lightRight();
      }
      delay(250);
      break;
    case '3': //3rd effect
      if(effect3 == 0){
        Console.println("Effect 3");
        Console.println("push any button");
        effect1 = 0;
        effect2 = 0;
        effect3 = 1;
      }
      pushRight = digitalRead(bRight);
      pushLeft = digitalRead(bLeft);

      if(pushRight == 1){
        right = 1;
        left = 0;
      }
  
      if(pushLeft == 1){
        left = 1;
        right = 0;
      } 
  
      if(left == 1){
        lightLeft();
      }
    
      if(right == 1){
       lightRight();
      }
      break;
  }

}

//turn on the led most of the left
void lightLeft(){
  
  int i;
  for(i = 0; i < sizeof(Leds)/sizeof(int); i++){
    digitalWrite(Leds[i],HIGH);
    delay(100);
    digitalWrite(Leds[i],LOW);
  }
}

//turn on the led most of the right
void lightRight(){
  
  int i;
  for(i = sizeof(Leds)/sizeof(int); i >= 0; i--){
    digitalWrite(Leds[i],HIGH);
    delay(100);
    digitalWrite(Leds[i],LOW);
  }
}
