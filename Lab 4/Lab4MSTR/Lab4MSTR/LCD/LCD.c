/*
 * LCD.c
 *
 * Created: 2/13/2025 7:18:53 PM
 *  Author: Fatima
 */ 


#include "LCD.h"

void initLCD_8bit(void){
	
	PORTD &= ~((1 << PORTD2) | (1 << PORTD3)); // RS y E apagado
	
	_delay_ms(20);
	CMD(0x30);
	_delay_ms(5);
	CMD(0x30);
	_delay_ms(5);
	CMD(0x30);
	_delay_ms(10);
	
	CMD(0x38); //Set de Funciones
	CMD(0x0C); //Display off
	CMD(0X01); //Display Clear
	CMD(0X06); //Entry mode
}

void CMD(char a){
	PORTD &= ~(1 << PORTD2); //Apagar RS
	Port(a);
	
	PORTD |= (1 << PORTD3); //Activar Enable
	_delay_ms(4);
	
	PORTD &= ~(1 << PORTD3); // Apagar enable
}

void Port(char a){
	//Primer bit
	if (a & 1){
		PORTD |= (1 << PORTD4);
	}
	else {
		PORTD &= ~(1 << PORTD4);
	}
	
	//Segundo bit
	if (a & 2){
		PORTD |= (1 << PORTD5);
	}
	else {
		PORTD &= ~(1 << PORTD5);
	}
	
	//Tercer bit
	if (a & 4){
		PORTD |= (1 << PORTD6);
	}
	else {
		PORTD &= ~(1 << PORTD6);
	}
	
	//Cuarto bit
	if (a & 8){
		PORTD |= (1 << PORTD7);
	}
	else {
		PORTD &= ~(1 << PORTD7);
	}
	
	//Quinto bit
	if (a & 16){
		PORTB |= (1 << PORTB0);
	}
	else {
		PORTB &= ~(1 << PORTB0);
	}
	
	//Sexto bit
	if (a & 32){
		PORTB |= (1 << PORTB1);
	}
	else {
		PORTB &= ~(1 << PORTB1);
	}
	
	//Septimo bit
	if (a & 64){
		PORTB |= (1 << PORTB2);
	}
	else {
		PORTB &= ~(1 << PORTB2);
	}
	
	//Quinto bit
	if (a & 128){
		PORTB |= (1 << PORTB3);
	}
	else {
		PORTB &= ~(1 << PORTB3);
	}
}

void WriteC(char a){	
	PORTD |= (1 << PORTD2); //encender rs para interpretar dato
	Port(a);
	
	PORTD |= (1 << PORTD3); // encender enable
	_delay_ms(4);
	
	PORTD &= ~(1 << PORTD3); // apagar enable
	
}

void WriteS(char *a){
	int i;
	for (i = 0; a[i] != '\0'; i++)
	WriteC(a[i]);
}

void cursor(char c, char f){
	char temp;
	
	if (f == 1){
		temp = 0x80 + c - 1;
		CMD(temp);
	} else if (f == 2){
		temp = 0xC0 + c -1;
		CMD(temp);
	}
}