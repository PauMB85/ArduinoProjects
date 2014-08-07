/***********************************************************
* Author: PauMB                                            *
* Program: turn on || turn off a led                       *
*          with a button                                   *
* Created: 08/05/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/

//digital pins
int button = 2;
int led = 8;

//variables
int pushNow = 0;
int pushLast = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);              //wait for open serial monitor
  
  pinMode(button,INPUT);
  pinMode(led,OUTPUT);
  
  Serial.println("Push the Button");

}

void loop() {
  // put your main code here, to run repeatedly:
  
  pushNow = digitalRead(button);

  if(pushNow == 1 && pushLast == 0){  //if push the button and the last stat is low, the led is on
    digitalWrite(led,HIGH);           //turn on
    Serial.println("Turn ON");
    pushLast = 1;
    pushNow = 0;
  }
  
  if(pushNow == 1 && pushLast == 1){  //if push the button and the las stat is high, the led is off 
    digitalWrite(led,LOW);            // turn off
    Serial.println("Turn OFF");
    pushLast = 0;
    pushNow = 0;
  }
  
  delay(250);
}
