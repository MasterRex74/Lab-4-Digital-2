/*
 * LCD.h
 *
 * Created: 2/13/2025 7:19:08 PM
 *  Author: Fatima
 */ 

#ifndef LCD_H_
#define LCD_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

void initLCD_8bit(void); //Iniciar LCD en modo 8 bits

void Port(char a); //Colocar valor en el puerto

void CMD(char a); // Colocar un comando

void WriteC(char a);

void WriteS(char *a);

void cursor(char c, char f);

#endif /* LCD_H_ */