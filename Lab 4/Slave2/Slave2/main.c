/*
 * Slave2.c
 *
 * Created: 13/02/2025 18:55:14
 * Author : Luis
 */ 

//SLAVE 2 CONTADOR

//*************************************************************************************************
//LIBRERIAS
//*************************************************************************************************
#define F_CPU 16000000

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "C:\Users\Juan Luis Monzón\Desktop\UVG\7 semestre\Digital 2\Lab 4\Slave2\Slave2\I2C\I2C.h"
//*************************************************************************************************
//VARIABLES GLOBALES
//*************************************************************************************************
#define SlaveAddress 0x30

uint8_t buffer = 0;
uint8_t contador = 0;
//*************************************************************************************************
//FUNCIONES
//*************************************************************************************************
void inc(void);
void dec(void);

//*************************************************************************************************
//MAIN
//*************************************************************************************************
int main(void)
{
setup_pines();
I2C_SLAVE_INIT(SlaveAddress);

//*****************************************INTERUPCIONES*******************************************
	TWCR |= (1 << TWIE);
	PCICR |= (1 << PCIE1);
	
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);
	
	sei();
	
    while (1) 
    {
    }
}
//*************************************************************************************************
//INTERRUPCIONES
//*************************************************************************************************
ISR(TWI_vect){						//INTERRUPCION DEL I2C
	uint8_t estado;
	estado = TWSR & 0xFC;
	switch(estado){
		case 0x60:
		case 0x70:
		TWCR |= (1 << TWINT);
		break;
		case 0x80:
		case 0x90:
		buffer = TWDR;
		TWCR |= (1 << TWINT);
		break;
		case 0xA8:
		case 0xB8:
		TWDR = contador;								//INFORMACION QUE SE LE MANDA AL MASTER                          
		TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
		break;
		default:
		TWCR |= (1 << TWINT) | (1 << TWSTO);
		break;
	}
}
ISR(PCINT1_vect){
	if (!(PINC & (1 << PINC0))){  // Botón en PD2 presionado (incrementar contador)
		contador++;
		if (contador > 15){
			contador = 0;  // Limitar a 4 bits
		}
	}
	if (!(PINC & (1 << PINC1))){  // Botón en PD3 presionado (decrementar contador)
		if (contador == 0){
			contador = 15;  // Si está en 0, ir a 15
			}else {
			contador--;
		}
	}

	// Actualizar salida en PORTB
	PORTB = (PORTB & 0xF0) | (contador & 0x0F);
}
//*************************************************************************************************
//FUNCIONES
//*************************************************************************************************

void setup_pines(){
	// Configurar los primeros 4 pines del puerto B como salida (contador de 4 bits)
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);  // PB0-PB3 como salida

	// Configurar pines de entrada para botones (por ejemplo, PD2 y PD3)
	DDRC &= ~((1 << DDC0) | (1 << DDC1));  // Entradas
	PORTC |= (1 << PORTC0) | (1 << PORTC1);    // Habilitar resistencias pull-up
}