/*Config LCD 4bits*/

//Librerias
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

/*------Pines LCD---------*/
//Pin enable (E)
#define E PORTB5
//Pin RS
#define RS PORTB4
//Pin DB4
#define DB4 PORTD4
//Pin DB5
#define DB5 PORTC6
//Pin DB6
#define DB6 PORTD7
//Pin DB7
#define DB7 PORTE6
//Botones LCD
#define botones 7

/*------------------------*/
/*------Pines modulo plazas parking*/
#define trig  PORTD3   //es un pin de salida
#define echo  PORTD2   //es un pin de entrada 
#define	estadoD	PIND   //para la lectura de los pines de entrada
//#define greenLed  PORTD2  //es un pin de salida
#define redLed  PORTB7    //es un pin de salida
/*----------------------------*/
/*-----Pines modulo entrada/salida parking-------*/
//motor que corresponde a la barrera de salida
#define motorOUT PORTB6  //corresponde al pin 10
//motor que corresponde a la barrera de entrada
//#define motorIN PORTB5  //corresponde al pin 9
//corresponde a los pines del pololu motor de entrada
//#define B1n PORTD7 //corresponde al pin 6
//#define A1n PORTD4 //corresponde al pin 4
//#define B2n PORTC7 //corresponde al pin 13
//#define A2n PORTC6 //corresponde al pin 5
#define B1n PORTD6 //coresponde al pin 12
#define A1n PORTC7 //corresponde al pin 13

/*---------Comandos-------*/
#define cursorHome         0x02 //envia el cursor en la posicion 0,0
#define clearLCD           0x01 //limpia la pantalla LCD
#define line1              0x80 // envia el cursor en la posicion 1 de la linea 1
#define line2              0xC0 //encia el cursor a la poscion 1 de la linea 2
#define shiftDisplayRight  0x1C //para desplazar la pantalla hacia la derecha
#define shiftDisplayLeft   0x18 //desplaza la pantalla a la izquierda
/*------------------------*/
unsigned char frase1[] = "Parking";
unsigned char frase6[] = "Disponible";
unsigned char frase2[] = "Parking Lleno";
unsigned char frase3[] = "Control Manual";
unsigned char frase4[] = "Abrir Entrada";
unsigned char frase5[] = "Cerrar Entrada";
unsigned char BotonLeft[] = "Motor salida";
unsigned char BotonUp[] = "Subir barrera";
unsigned char BotonDown[] = "Bajar barrera";
unsigned char BotonRight[] = "Motor entrada";
/*variables globales*/
//plaza parking
double r;
double d;
int result;
//LCD
int parking = 10;
//entrada salida parling
boolean abrir1 = false;
boolean cierra1 = false;
boolean abrir2 = false;
boolean cierra2 = false;
boolean izquierdo = false;
boolean derecho = false;

void setup() {
  //inicializacion LCD
  inicializarPuertos();
  inicializarLCD();
  inicializarADC();
  sendDatosLCD(0x0C);
  sendDatosLCD(clearLCD);
  /*
  //inicializacion Sensor distancia
  HCSR04_init();
  LEDS_init();
  TIMER3_init();
  */
  //inicializamos entrada/salida parking
  PIN_init();
  PWM_init();
  interrupt_Init();
}

void loop() {
  //indica si el parking esta disponible o no
   if( parking == 0)
   {
     sendDatosLCD(clearLCD);
     sendStringLCD(frase2);
     _delay_ms(2000);
   }
   else
   {
     sendDatosLCD(clearLCD);
     sendStringLCD(frase1);
     sendDatosLCD(0xC0);
     sendStringLCD(frase6);
     _delay_ms(2000);
   }
   
   // para saber si la plaza esta libre o ocupada
   //Enviamos el pulso trigger
   TRIGGER();
  //esperamos la respuesta del pulso enviado, echo
   r = ECHO();
   //Calculamos la distancia que hay con el objeto detectado
   d=(r/58.0);	
   _delay_ms(500);
  
  if( (d > -1) & (d < 200))
  {
      //Al detectar un obstaculo significa que hay un vehiculo 
      //por lo tanto el led verde debe estar apagado y el led rojo encendido
      //PORTD &= ~(1 << greenLed);
      //PORTD |= (1 << redLed);
     PORTB |=(1 << redLed); 
  }
  else
  {

    //Al no haber un obtaculo significa que la plaza esta libre
    //por lo tanto el led verde debe estar encendido y el led rojo apagado
    //PORTD |= (1 << greenLed);
    //PORTD &= ~(1 << redLed);
    PORTB &= ~(1 << redLed);
  }
  
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

/* Funcion que inicializa los pines del LCD*/
void inicializarPuertos()
{
  //Puerto B
  //Se inicializan los puertos 4(RS) y el 5(E) 
  DDRB = 0x30;//solo con RS y E
  PORTB = 0;

  //Puerto C
  //Se inicializa el puerto 6(DB5)
  DDRC =  0x40;
  PORTC = 0;

  //Puerto D
  //Se inicializan los puertos 4(DB4) y 7(DB6) y 6(led rojo)
  //DDRD =  0x90; solo para los puertos 4 y 7
  DDRD = 0xD0;//
  PORTD = 0;
  
  //Puerto E
  //Se inicializa el puerto 6(DB7)
  DDRE =  0x40;
  PORTE = 0;

}
/*
  Funcion que inicializar el LCD
*/
void inicializarLCD()
{
  _delay_ms(100);
  funtionSet8();
  _delay_ms(100);
  funtionSet8();
  _delay_ms(10);
  funtionSet8();
  funtionSet4();
  sendDatosLCD(0x28); //FuntionSet4 0010NF** --> 00101000
  sendDatosLCD(0x08); //Display on/off 00001000
  sendDatosLCD(clearLCD); //Display Clear 00000001
  sendDatosLCD(0x06); //Entry Mode Set 000001I/DS --> 00000100
  
}

/*
  Funcion enable
*/
void enable()
{
  PORTB |= (1 << E);
  _delay_us(2);
  PORTB &= (~1 << E);
  _delay_ms(2);
}

/*
  Function Set de 8 bits,
  necesario para inicializar LCD
*/
void funtionSet8()
{
  //RS = 0
  PORTB &= ~(1 << RS);
  //Datos 0011
  PORTE &= ~(1 << DB7);
  PORTD &= ~(1 << DB6);
  PORTC |= (1 << DB5);
  PORTD |= (1 << DB4);
  //Enable
  enable();
}

/*
  Function Set de 4 bits,
  necesario para inicializar LCD
*/
void funtionSet4()
{
  //RS = 0
  PORTB &= ~(1 << RS);
  //Datos 0010
  PORTE &= ~(1 << DB7);
  PORTD &= ~(1 << DB6);
  PORTC |= (1 << DB5);
  PORTD &= ~(1 << DB4);
  //Enable
  enable();
}

/*
  Funcion que se encarga de enviar los
  comandos al LCD
*/
void sendDatosLCD(unsigned char dato)
{
  unsigned char dataHigh = (dato & 0xF0) >> 4;
  unsigned char dataLow = (dato & 0x0F);

  //Enviamos la parte alta
  //RS = 0
  PORTB &= ~(1 << RS);
  sendNibble(dataHigh);
  enable();

  //Enviamos la parte baja
  //RS = 0
  PORTB &= ~(1 << RS);
  sendNibble(dataLow);
  enable();

}

/*
  Funcion que se encarga de imprimir
  un caracter al LCD
*/
void sendCharLCD(unsigned char letra)
{
  unsigned char dataHigh = (letra & 0xF0) >> 4;
  unsigned char dataLow = (letra & 0x0F);

  //Enviamos la parte alta
  //RS = 1
  PORTB |= (1 << RS);
  sendNibble(dataHigh);
  enable();

  //Enviamos la parte baja
  //RS = 1
  PORTB |= (1 << RS);
  sendNibble(dataLow);
  enable();

}

/*
  Funcion que se encarga de imprimir un
  cadena de caracteres al LCD
*/
void sendStringLCD(unsigned char *cadena)
{
  char i = 0;
  unsigned char *inicio = cadena;
  unsigned char letra;

  while (inicio[i] != '\0')
  {
    letra = inicio[i];
    sendCharLCD(letra);
    i++;
  }
}

/*
  Funcion que se encarga de poner 0 o 1
  en los pines correspondientes al LCD
*/
void sendNibble(unsigned char nible)
{
  //DB7
  if ((nible & 0x8) >> 3 == 1)
    PORTE |= (1 << DB7);
  else
    PORTE &= ~(1 << DB7);

  //DB6
  if ((nible & 0x4) >> 2 == 1)
    PORTD |= (1 << DB6);
  else
    PORTD &= ~(1 << DB6);

  //DB5
  if ((nible & 0x2) >> 1 == 1)
    PORTC |= (1 << DB5);
  else
    PORTC &= ~(1 << DB5);

  //DB4
  if ((nible & 0x1) == 1)
    PORTD |= (1<<DB4);
  else
    PORTD &= ~(1 << DB4);

} 

/*
  Funcion que se encarga de posicionar el cursor
  El eje Y significa la linea
  El eje X significa la posicion de la linea
*/
void positionCursor(unsigned char x, unsigned char y)
{
  unsigned char posicion;
  switch(y)
  {
    case 1:
      posicion = line1+(x-1);
      break;
    case 2:
      posicion = line2+(x-1);
      break;
  }
  sendDatosLCD(posicion);
}

/*
  Funcion que se encarga de desplazar el display
*/
void shiftDisplay(unsigned char shift)
{
  //se desplaza la pantalla 16 posiciones
  int i = 0;
  while(i<16)
  {
    sendDatosLCD(shift);
    i++;
  }
}

/*Inicializamos el puerto ADC*/
void inicializarADC()
{
  ADCSRA |= 1<<ADPS2;//preescaler 16
  ADMUX |= 1<<REFS0 | 1<<REFS1; // Aref interno
  ADMUX |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0);// adc canal 7
  ADCSRA |= 1<<ADIE;//habilitar interrupcion,el valor se guarda en ADC(L/H)
  ADCSRA |= 1<<ADEN;//habilitamos ADC
  ADCSRA |= 1<<ADSC;//se haran conversiones automaticamente
}

ISR(ADC_vect)
{
  uint8_t theLow = ADCL;//valor leido en el puertp adcLOW
  uint16_t theTenBitResult = ADCH<<8 | theLow; //valor ADC high + low
  
  if((theTenBitResult > 650) && (theTenBitResult < 950))
  {
    //motor de salida
    sendDatosLCD(clearLCD);
    sendStringLCD(BotonLeft);
    sendDatosLCD(line2);
    sendStringLCD(frase3);
    izquierdo = true;
    derecho = false;
  }
  else if((theTenBitResult > 400) && (theTenBitResult < 500))
  {
    //bajamos barrera
    sendDatosLCD(clearLCD);
    sendStringLCD(BotonDown);
    sendDatosLCD(line2);
    sendStringLCD(frase3);
    if (derecho)
      cierra1 = true;
    else
      cierra2= true;
  }
  else if((theTenBitResult > 100) && (theTenBitResult < 200))
  {
    //subimos barrera
    sendDatosLCD(clearLCD);
    sendStringLCD(BotonUp);
    sendDatosLCD(line2);
    sendStringLCD(frase3);
    if (izquierdo)
      abrir2 = true;
    else
      abrir1 = true;
  }
  else if((theTenBitResult >= 0)&& theTenBitResult < 100)
  {
    //motor entrada
    sendDatosLCD(clearLCD);
    sendStringLCD(BotonRight);
    sendDatosLCD(line2);
    sendStringLCD(frase3);
    derecho = true;
    izquierdo = false;
  }

  ADCSRA |= 1<<ADSC;
  
}

/* inicializa los pines del sensor de distancia*/
void HCSR04_init()
{
  //Inicializamos el puerto D0
  DDRD |= (1 << trig);  //como solo trig es de salida ponemos un 1 en PD0
  PORTD = 0; //ponemos un cero todos los pines del puerto D, para inicializar
}

/*funcion que inicializa los puertos de los leds*/
void LEDS_init()
{
  /*
  //Inicializamos los puertos PD3(led verde) y el PD2(led rojo)
  DDRD |= (1 << greenLed) | (1 << redLed);
  PORTD = 0; //ponemos todos los pines del puerto D a zero
  
  //iniciamos los leds con el led verde encendido y el led rojo apagado
  PORTD |= (1 << greenLed);
  PORTD &= ~(1 << redLed);
  */
  //NUEVA REASIGNACION DEL LED EN EL PIN 11
  DDRB |= (1 << redLed);
  PORTD &= ~(1 << redLed);
}

/*Funcion que inicializa el timer3*/
void TIMER3_init()
{
  //iniciamos el timer3 para calcular el tiempo que dura el pulso
  TCCR3A=0;
  TCCR3B=(1<<CS31);	//Prescaler = Fcpu/8
}

void TRIGGER()
{
  //Enviamos un pulso TTL
  PORTD |= (1 << trig);
  _delay_us(15); //esperamos 15us
  PORTD &= ~(1 << trig);//finalizamos el pulso.
}

int ECHO()
{
  result = 0;

  //esperamos a que se detecte el pulso de entrada, se espera a que sea nivel alto
  while(!(estadoD & (1<<echo)));
 
  TCNT3=0x00;//Iniciamos el timer a 0

  //Calculamos el ancho del pulso
  while(estadoD & (1<<echo))
  {
     if(TCNT3 > 60000)
     {
       //si superamos los 30ms es debido a que no se encuentra ningun objeto
       return -1;
     }
  }
  
  result = TCNT3;
  return (result>>1);//devolvemos el resultado del tiempo ya dividido entre 2.
}

/* Inicializamos los pines de los motores y su control*/
void PIN_init()
{
  //Pines de salida
  /*DDRD |= (1 << B1n) | (1 << A1n);
  DDRC |= (1 << B2n) | (1 << A2n);
  DDRB |= (1 << motorIN) | (1 << motorOUT);
  */
  //Con la nueva reasignacion
  DDRD |= (1 << B1n);
  DDRC |= (1 << A1n);
  DDRB |= (1 << motorOUT);
  
}

/*configuacion del pwm*/
void PWM_init()
{
  //utilizaremos el timer1
   //Registro TCCR1A
   //TCCR1A |= (1 << COM1A1) | (1 << COM1B1); //habilitamos los del timer a y b
   TCCR1A |= (1 << COM1B1);
   TCCR1A |= (WGM10); //fast pwm 8-bits
   //Registro TCCR1B
   TCCR1B |= (1 << WGM12);// | (1 << WGM13); // fast pwm 8-bits
   TCCR1B |= (1 << CS10); //preescaler 1
}
/*PWM del motor entrada o salida, funcion de n*/
void SetPWMOutput(uint8_t duty, int n)
{
  /*if(n == 1)
    OCR1A=duty;
  else*/
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
    //PORTD |= (1 << A1n);
    //PORTD &= ~(1 << B1n);
  }
  else
  {
    PORTC |= (1 << A1n);
    PORTD &= ~(1 << B1n);
    //PORTC |= (1 << A2n);
    //PORTC &= ~(1 << B2n);
  }
}

/*gira el motor de entrada en sentido antihorario*/
void direccionAntihorario(int m)
{
  
  if(m == 1)
  {
    //PORTD &= ~(1 << A1n);
    //PORTD |= (1 << B1n);
  }
  else
  {
    //PORTC &= ~(1 << A2n);
    //PORTC |= (1 << B2n);
    PORTC &= ~(1 << A1n);
    PORTD |= (1 << B1n);
  }

}

/*funcion que frena el motor de entrada*/
void freno(int m)
{
  if(m == 1)
  {
    //PORTD |= (1 << A1n);
    //PORTD |= (1 << B1n);
  }
  else
  {
    //PORTC |= (1 << A2n);
    //PORTC |= (1 << B2n);
    PORTC |= (1 << A1n);
    PORTC |= (1 << B1n);
  }
}

/*Habilitamos las interrupciones externas*/
void interrupt_Init()
{
  
  //habilitamos interrupciones externas 0
  EIMSK |= (1 << INT0);
  //habilitamos interrupciones externas 1
  EIMSK |= (1 << INT1);
  //habilitamos interrupciones externas 2
  //EIMSK |= (1 << INT2);
  //habilitamos interrupciones externas 2
  //EIMSK |= (1 << INT3);
  
  //Modo de interrupcion
  EICRA |= (1 << ISC01) | (1 << ISC00); //habilitamos la interrupcion externa 0 con flanco de subida
  EICRA |= (1 << ISC11) | (1 << ISC10); //habilitamos interrupcion externa 1 con flanco de subida
  //EICRA |= (1 << ISC21) | (1 << ISC20); //habilitamos interrupcion externa 2 con flanco de subida
  //EICRA |= (1 << ISC31) | (1 << ISC30); //habilitamos interrupcion externa 3 con flanco de subida
}

/*interrupcion externa 0*/
ISR(INT0_vect)
{

  //abrir1=true;
  abrir2=true;
}

/*interrupcion externa 1*/
ISR(INT1_vect)
{

  //cierra1=true;
  cierra2=true;
}
/*
//interrupcion externa 2
ISR(INT2_vect)
{

  abrir2=true;
}
//interrupcion externa 3
ISR(INT3_vect)
{
  
  cierra2=true;
}
*/
