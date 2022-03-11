/*
Cotroladores y Microcontroladores Programables - Jueves M4-M6.
Autor: Fabián Eduardo Cruz Amador.
N.L:9.
Creado con PlatformIO (Visual Studio Code).
*/

#include <time.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 1000000UL // Velocidad oscilador interno 1Mhz

// Display
#define DISPLAY PORTD

// Valores del display
uint8_t display[11] = {
    0xFC, // 0
    0x60, // 1
    0xDA, // 2
    0xF2, // 3
    0x66, // 4
    0xB6, // 5
    0xBE, // 6
    0xE0, // 7
    0xFE, // 8
    0xE6, // 9
    0x9E  // E
};

#define BUTTONS PINC
// Button0
#define BUTTON0 PINC0
#define BUTTON0_PRESS bit_is_clear(BUTTONS, BUTTON0)
// Button1
#define BUTTON1 PINC1
#define BUTTON1_PRESS bit_is_clear(BUTTONS, BUTTON1)
// Button2
#define BUTTON2 PINC2
#define BUTTON2_PRESS bit_is_clear(BUTTONS, BUTTON2)

// Condiciones
#define case00 !BUTTON1_PRESS && !BUTTON0_PRESS
#define case10 BUTTON1_PRESS && !BUTTON0_PRESS
#define case01 !BUTTON1_PRESS &&BUTTON0_PRESS
#define case11 BUTTON1_PRESS &&BUTTON0_PRESS
#define stop BUTTON2_PRESS

int main(void)
{
  // Se declaran los 6 leds (PB0-PB5).
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);

  // Se declara desplay.
  DDRD = 0xFF;

  // Se declaran los 3 botones (PC0 Y PC1).
  DDRC &= ~((1 << BUTTON0) | (1 << BUTTON1) | (1 << BUTTON2));
  PORTC |= _BV(BUTTON0) | _BV(BUTTON1) | _BV(BUTTON2);

  while (1)
  {
    int i = 0;
    if (case00)
    {
      // Leds encendidos completamente y el display 7 segmentos muestra un “E” para indicar que este encendido
      PORTB = 0b00111111;
      PORTD = display[10];
    }
    else if (case01)
    {
      // Leds muestran un contador descendente de 6 bits y el display muestra una “1”, al llegar a su tope el conteo se reinicia
      PORTD = display[1];
      i = 63;
      while (1)
      {
        if (!(case01))
        {
          break;
        }

        if (i >= 0)
        {
          PORTB = i;
          _delay_ms(20);
          if (stop) // BOTON DE STOP SE PRESIONÓ
            i = i;
          else
            i--;
        }
        else
        {
          i = 63;
        }
      }
    }
    else if (case10)
    {
      // Leds muestran un contador ascendente de 6 bits y el display muestra una “2”, al llegar a su tope el conteo se reinicia
      PORTD = display[2];
      i = 0;
      while (1)
      {
        if (!(case10))
        {
          break;
        }

        if (i < 64)
        {
          PORTB = i;
          _delay_ms(20);
          if (stop) // BOTON DE STOP SE PRESIONÓ
            i = i;
          else
            i++;
        }
        else
        {
          i = 0;
        }
      }
    }
    else if (case11)
    {
      // Los leds de los extremos se apagan, los demás se quedan encendidos y el display muestra un contador descendente de 9 a 0, al llegar a su tope el conteo se reinicia
      PORTB = 0b00011110;
      i = 9;
      while (1)
      {
        if (!(case11))
        {
          break;
        }
        if (i >= 0)
        {
          PORTD = display[i];
          _delay_ms(40);
          if (stop) // BOTON DE STOP SE PRESIONÓ
            i = i;
          else
            i--;
        }
        else
        {
          i = 9;
        }
      }
    }
  }
}