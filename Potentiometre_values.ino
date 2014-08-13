/***********************************************************
* Author: PauMB                                            *
* Program: Read potentiometer's values                     *
* Created: 08/07/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/

int pot = 5; //A5

int valuePot = 0; //Value of the potentiometer
long valuePotOHMS = 0; //the real value of the potentiometer

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //inicialize serial
  pinMode(pot,INPUT);  //configure the pin
  
  while(!Serial){//waitng for the monitor serial
  }
  
  Serial.println("Read potenciometre's values");

}

void loop() {
  // put your main code here, to run repeatedly:
  valuePot = analogRead(pot); //read analog input, potentiometre
  valuePotOHMS = map(valuePot,0,1014,0,99999); // map the valuePot 
  
  Serial.print(valuePot); //print the value
  Serial.print(" --> ");
  Serial.print(valuePotOHMS);
  Serial.println(" ohms");
  
  delay(2000);

}
