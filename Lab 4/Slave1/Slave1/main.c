/*
 * Slave1.c
 *
 * Created: 13/02/2025 18:29:37
 * Author : Luis
 */ 

//SLAVE 1 ADC

//*************************************************************************************************
//LIBRERIAS
//*************************************************************************************************
#define F_CPU 16000000

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "C:\Users\Juan Luis Monzón\Desktop\UVG\7 semestre\Digital 2\Lab 4\Slave1\Slave1\I2C\I2C.h"
#include "C:\Users\Juan Luis Monzón\Desktop\UVG\7 semestre\Digital 2\Lab 4\Slave1\Slave1\ADC\ADC.h"
//*************************************************************************************************
//VARIABLES GLOBALES
//*************************************************************************************************
#define SlaveAddress 0x40
uint8_t POT = 0;
uint8_t buffer = 0;

//*************************************************************************************************
//MAIN
//*************************************************************************************************
int main(void){
	
//*****************************************ADC*****************************************************
	setADC(externo, izquierda, 7);
	controlADC(1, 0, 1, 128);

	I2C_SLAVE_INIT(SlaveAddress);

//************************************INTERRUPCIONES***********************************************
	TWCR |= (1 << TWIE);
	sei();

	while (1) 
    {
		ADCSRA |= (1 << ADSC);
		
    }
}
//*************************************************************************************************
//INTERRUPCIONES
//*************************************************************************************************

ISR(ADC_vect){
	POT = ADCH;
	
	ADCSRA |= (1 << ADIF); //Apagar la bandera de ADC
}
ISR(TWI_vect){
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
			TWDR = POT;
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
			break;
		default:
			TWCR |= (1 << TWINT) | (1 << TWSTO);
			break;
	}
}