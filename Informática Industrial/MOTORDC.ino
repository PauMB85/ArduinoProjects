#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

//motor que corresponde a la barrera de salida
#define motorOUT PORTB6  //corresponde al pin 10
//motor que corresponde a la barrera de entrada
#define motorIN PORTB5  //corresponde al pin 9

//corresponde a los pines del pololu motor de entrada
#define B1n PORTD7 //corresponde al pin 6
#define A1n PORTD4 //corresponde al pin 4
#define B2n PORTC7 //corresponde al pin 13
#define A2n PORTC6 //corresponde al pin 5

//variables globales
boolean abrir1 = false;
boolean cierra1 = false;
boolean abrir2 = false;
boolean cierra2 = false;

void setup() {
  // put your setup code here, to run once:
  //inicializacion pines
  Serial.begin(9600);
  while(!Serial);
  PIN_init();
  //inicializacion PWM
  PWM_init();
  //inicializamos interrupciones externas
  interrupt_Init();

}

void loop() {
  // put your main code here, to run repeatedly:
  //control del motor 1
  if (abrir1)
  {
    sube(1);
    abrir1 = false;
   
  }
  if(cierra1)
  {
    baja(1);
    cierra1 = false;
  }
  //control del motor 2
  if(abrir2)
  {
    sube(2);
    abrir2 = false;
  }
  if(cierra2)
  {
    baja(2);
    cierra2 = false;
  }

}

void PIN_init()
{
  //Pines de salida
  DDRD |= (1 << B1n) | (1 << A1n);
  DDRC |= (1 << B2n) | (1 << A2n);
  DDRB |= (1 << motorIN) | (1 << motorOUT);
  
}

void PWM_init()
{
  //utilizaremos el timer1
   //Registro TCCR1A
   TCCR1A |= (1 << COM1A1) | (1 << COM1B1); //habilitamos los del timer a y b
   TCCR1A |= (WGM10); //fast pwm 8-bits
   //Registro TCCR1B
   TCCR1B |= (1 << WGM12);// | (1 << WGM13); // fast pwm 8-bits
   TCCR1B |= (1 << CS10); //preescaler 1
}
/*PWM del motor entrada o salida, funcion de n*/
void SetPWMOutput(uint8_t duty, int n)
{
  if(n == 1)
    OCR1A=duty;
  else
    OCR1B=duty;
}


void sube(int m)
{ 
  direccionHoraria1(m);
  
  SetPWMOutput(50,m);
  _delay_ms(2500);
  SetPWMOutput(40,m);
  _delay_ms(200);
  SetPWMOutput(30,m);
  _delay_ms(200);
  SetPWMOutput(20,m);
  _delay_ms(200);
  SetPWMOutput(10,m);
  _delay_ms(200);
  SetPWMOutput(5,m);
  _delay_ms(200);
  
  freno(m);

}

void baja(int m)
{
 //motor gira sentido antihorario
  direccionAntihorario(m);
  SetPWMOutput(50,m);
  _delay_ms(2500);
  SetPWMOutput(40,m);
  _delay_ms(200);
  SetPWMOutput(30,m);
  _delay_ms(200);
  SetPWMOutput(20,m);
  _delay_ms(200);
  SetPWMOutput(5,m);
  _delay_ms(200);
  SetPWMOutput(5,m);
  _delay_ms(100);
   //Freno
   freno(m);
}

/*gira el motor en sentido horario*/
void direccionHoraria1(int m)
{
  //motor gira sentido horario
  if(m == 1)
  {
    PORTD |= (1 << A1n);
    PORTD &= ~(1 << B1n);
  }
  else
  {
    PORTC |= (1 << A2n);
    PORTC &= ~(1 << B2n);
  }
}

/*gira el motor de entrada en sentido antihorario*/
void direccionAntihorario(int m)
{
  if(m == 1)
  {
    PORTD &= ~(1 << A1n);
    PORTD |= (1 << B1n);
  }
  else
  {
    PORTC &= ~(1 << A2n);
    PORTC |= (1 << B2n);
  }

  
}
/*funcion que frena el motor de entrada*/
void freno(int m)
{
  if(m == 1)
  {
    PORTD |= (1 << A1n);
    PORTD |= (1 << B1n);
  }
  else
  {
    PORTC |= (1 << A2n);
    PORTC |= (1 << B2n);
  }
    
  

}

/*Habilitamos las interrupciones externas*/
void interrupt_Init()
{
  
  //habilitamos interrupciones externas 0
  //EIMSK |= (1 << INT0);
  //habilitamos interrupciones externas 1
  EIMSK |= (1 << INT1);
  //habilitamos interrupciones externas 2
  //EIMSK |= (1 << INT2);
  //habilitamos interrupciones externas 2
  //EIMSK |= (1 << INT3);
  
  //Modo de interrupcion
  //EICRA |= (1 << ISC01) | (1 << ISC00); //habilitamos la interrupcion externa 0 con flanco de subida
  EICRA |= (1 << ISC11) | (1 << ISC10); //habilitamos interrupcion externa 1 con flanco de subida
  //EICRA |= (1 << ISC21) | (1 << ISC20); //habilitamos interrupcion externa 2 con flanco de subida
  //EICRA |= (1 << ISC31) | (1 << ISC30); //habilitamos interrupcion externa 3 con flanco de subida
}

/*interrupcion externa 0*/
ISR(INT0_vect)
{
  cli();//deshabilitamos interrupciones
  abrir1=true;
  sei();
}

/*interrupcion externa 1*/
ISR(INT1_vect)
{
  cli();
  cierra1=true;
  sei();
}
/*interrupcion externa 2*/
ISR(INT2_vect)
{
  cli();
  abrir2=true;
  sei();
}
/*interrupcion externa 3*/
ISR(INT3_vect)
{
  cli();
  cierra2=true;
  sei();
}
