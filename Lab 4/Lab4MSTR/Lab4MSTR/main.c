/// Universidad del Valle de Guatemala
// Electrónica Digital 2
// main.c
// Proyecto: Lab2
// Hardware: ATMEGA328P
// Created: 1/23/2025
// Author : Fatima Urrutia
//********************************************************************************************************************************************

//********************************************************************************************************************************************
// Librerias
//********************************************************************************************************************************************

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "C:\Users\Fatima\Documents\4to año\1er ciclo\Digital2\Lab2\Lab2\LCD\LCD.h"
#include "C:\Users\Fatima\Documents\4to año\1er ciclo\Digital2\Lab4MSTR\Lab4MSTR\I2C\I2C.h"

#define slaveadc 0x40
#define slavecont 0x30

//********************************************************************************************************************************************
// Variables Globales
//********************************************************************************************************************************************

uint8_t add;
uint8_t temp;
uint8_t buffer;
uint8_t valor = 0;

float volt = 0;

char buf[20];

//********************************************************************************************************************************************
// Declarar Funciones
//********************************************************************************************************************************************

void setup(void);
void floatToString(float num, char *str, int precision);
void initUART9600(void);

//********************************************************************************************************************************************
// Main
//********************************************************************************************************************************************

int main(void)
{
    setup();
	cursor(4,1);
	WriteS("S1:");
	
	cursor(11,1);
	WriteS("S2:");
	
	volt = valor * 5.0 / 255;
	floatToString(volt, buf, 2);
	cursor(4,2);
	WriteS(buf);
	WriteC('V');
	
	itoa(valor, buf, 10);
	cursor(11,2);
	WriteS(buf);
	
    while (1) 
    {
		/*I2C_MASTER_START();  //Mandar el start al maestro
		buffer = slavecont << 1 & 0b11111110; //Preparar la direccion escribir
		
		temp = I2C_MASTER_WRITE(buffer); //Guardar resultado anterior en una variable temporal
		
		if(temp != 1){
			itoa(temp, buf, 10);
			cursor(1,1);
			WriteS(buf);
			I2C_MASTER_STOP(); //Parar si hay un error
		} else{
			itoa(temp, buf, 10);
			cursor(1,1);
			WriteS(buf);
			I2C_MASTER_WRITE('P'); //Enviar "comando"
			I2C_MASTER_STOP(); //Paro la comunicacion
			_delay_ms(500);
			*/
			I2C_MASTER_START();  //Mandar el start al maestro
			buffer = slaveadc << 1 | 0b00000001; //Preparar la direccion para leer
			temp = I2C_MASTER_WRITE(buffer);
			if (temp != 1){
				I2C_MASTER_STOP(); //Parar si hay un error
			} else{
				TWCR |= (1 << TWINT) | (1 << TWEN);	//INICIAMOS LA LECTURA
				while(!(TWCR & (1 << TWINT))); //ESPERAMOS LA BANDERA
				
				valor = TWDR;
				I2C_MASTER_STOP();
				
				volt = valor * 5.0 / 255;
				floatToString(volt, buf, 2);
				cursor(4,2);
				WriteS(buf);
				WriteC('V');
				_delay_ms(200);
			}
			
			I2C_MASTER_START();
			buffer = slavecont << 1 | 0b00000001; //Preparar la direccion para leer contador
			temp = I2C_MASTER_WRITE(buffer);
			if (temp != 1){
				I2C_MASTER_STOP(); //Parar si hay un error
				} else{
				TWCR |= (1 << TWINT) | (1 << TWEN);	//INICIAMOS LA LECTURA
				while(!(TWCR & (1 << TWINT))); //ESPERAMOS LA BANDERA
				
				valor = TWDR;
				I2C_MASTER_STOP();
				
				/*itoa(valor, buf, 10);
				cursor(11,2);
				WriteS(buf);
				_delay_ms(200);*/
				
				itoa(valor, buf, 10);
				if (valor < 10){
					cursor(11,2);
					WriteS("0");
					cursor(12,2);
					WriteS(buf);
				}
				else if (valor >= 10)
				{
					cursor(11,2);
					WriteS(buf);
				}
				_delay_ms(200);
			}
					
    }
}

//********************************************************************************************************************************************
// Setup
//********************************************************************************************************************************************

void setup(void){
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);  //Setear las salidas en puerto d
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
	
	//----------------------- LCD ------------------------------
	
	initLCD_8bit();
	
	//----------------------- I2C ------------------------------
	
	I2C_MASTER_INIT(100000, 1); //100KHz y prescaler de 1
	
	sei(); 
}

//********************************************************************************************************************************************
// Sub-Funciones
//********************************************************************************************************************************************

void floatToString(float num, char *str, int precision) {
	int intPart = (int)num;  // Parte entera
	float decimalPart = num - intPart;  // Parte decimal

	// Convertir parte entera con itoa()
	itoa(intPart, str, 10);

	// Agregar punto decimal
	int i = 0;
	while (str[i] != '\0') i++;  // Buscar el final de la cadena
	str[i++] = '.';

	// Convertir parte decimal manualmente
	for (int j = 0; j < precision; j++) {
		decimalPart *= 10;
		int decDigit = (int)decimalPart;
		str[i++] = decDigit + '0';
		decimalPart -= decDigit;
	}

	str[i] = '\0';  // Terminar la cadena
}



//********************************************************************************************************************************************
// Interrupciones
//********************************************************************************************************************************************



