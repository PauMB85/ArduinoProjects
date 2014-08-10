/***********************************************************
* Author: PauMB                                            *
* Program: turn on || turn of a led,                       *
*          and regulation light's intensity in function    *
*          of the LDR sensor                               *
* Created: 08/02/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/

int LDR = 1; // read the analog pin A1
int LED = 11; // turn on || turn of the led in the pin 11 (pwm)
int valLDR = 0; //the value of the LDR
int lightLED; //intensity of the led

void setup() {
  
  // initialize serial communication:
  Serial.begin(9600); 
  pinMode(LED, OUTPUT); //inicialize the led output

}

void loop() {
  
  //read the value of LDR
  valLDR = analogRead(LDR);
  Serial.println(valLDR); //print LDR's value
  
  if(valLDR < 250)  //possible you need change this value
  {
    //regulate the led in funtion of LDR's value
    lightLED = (valLDR/2)-150;
    analogWrite(LED,lightLED); // turn on the led, pwm pin 11
  }
  else
    analogWrite(LED, LOW); //turn of the led
  
  delay(100);
  
}
