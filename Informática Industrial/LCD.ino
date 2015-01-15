/*Config LCD 4bits*/

//Librerias
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>


//Frecuencia
#define F_CPU 8000000L

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
//Led rojo
#define LRojo PORTD6
/*------------------------*/

/*---------Comandos-------*/
#define cursorHome         0x02 //envia el cursor en la posicion 0,0
#define clearLCD           0x01 //limpia la pantalla LCD
#define line1              0x80 // envia el cursor en la posicion 1 de la linea 1
#define line2              0xC0 //encia el cursor a la poscion 1 de la linea 2
#define shiftDisplayRight  0x1C //para desplazar la pantalla hacia la derecha
#define shiftDisplayLeft   0x18 //desplaza la pantalla a la izquierda
/*------------------------*/
unsigned char frase1[] = "Parking Disponible";
unsigned char frase2[] = "Parking Lleno";
unsigned char frase3[] = "Control Manual";
unsigned char frase4[] = "Abrir Entrada";
unsigned char frase5[] = "Cerrar Entrada";
unsigned char BotonSelect[] = "Boton Select";
unsigned char BotonLeft[] = "Motor salida";
unsigned char BotonUp[] = "Subir barrera";
unsigned char BotonDown[] = "Bajar barrera";
unsigned char BotonRight[] = "Motor entrada";

int parking = 10; //supuesto numero de plazas dle parking
void setup() {
  
  inicializarPuertos();
  inicializarLCD();
  inicializarADC();
  //sei();
  sendDatosLCD(clearLCD);
  sendStringLCD(frase3);
  delay(2000);
  sendDatosLCD(clearLCD);
  sendStringLCD(frase4);
  delay(2000);
  sendDatosLCD(clearLCD);
  sendCharLCD('S');
  sendCharLCD('i');
  delay(2000);
  sendDatosLCD(clearLCD);
  sendStringLCD(frase2);
  delay(2000);
  sendDatosLCD(0xCC);
  delay(2000);
  positionCursor(3,2);
  delay(2000);
  sendStringLCD(frase5);

  delay(2000);
  shiftDisplay(shiftDisplayLeft);
  delay(2000);
  positionCursor(17,1);
  delay(2000);
  sendStringLCD(frase1);
  delay(2000);
  //sendDatosLCD(cursorHome);
  shiftDisplay(shiftDisplayRight);
}

void loop() {
  // put your main code here, to run repeatedly:
   if( parking == 0)
     sendDatosLCD(clearLCD);
     sendStringLCD(frase2);
   else
     sendDatosLCD(clearLCD);
     sendStringLCD(frase1);
}


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
  //inicializamos el puerto 6 
  //DDRE = 0x
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
  sendDatosLCD(0x0F); //Visualizar cursos 00001DUB --> 00001111, para ayudar hay que eliminar
  
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

void inicializarADC()
{
  ADCSRA |= 1<<ADPS2;
  ADMUX |= 1<<REFS0 | 1<<REFS1;
  ADMUX |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0);// adc canal 7
  ADCSRA |= 1<<ADIE;
  ADCSRA |= 1<<ADEN;

  ADCSRA |= 1<<ADSC;
}

ISR(ADC_vect)
{
  uint8_t theLow = ADCL;
  uint16_t theTenBitResult = ADCH<<8 | theLow;
  
  if((theTenBitResult < 1020)&&(theTenBitResult > 950)){
    sendStringLCD(BotonSelect);
  }
  else if((theTenBitResult > 650) && (theTenBitResult < 950))
  {
    sendStringLCD(BotonLeft);
  }
  else if((theTenBitResult > 400) && (theTenBitResult < 500))
  {
    sendStringLCD(BotonDown);
  }
  else if((theTenBitResult > 100) && (theTenBitResult < 200))
  {
    sendStringLCD(BotonUp);
  }
  else if((theTenBitResult >= 0)&& theTenBitResult < 100)
  {
    sendStringLCD(BotonRight);
  }

  ADCSRA |= 1<<ADSC;
  
}
