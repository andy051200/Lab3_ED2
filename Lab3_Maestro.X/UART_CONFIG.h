/* 
 * File:   uart_config.h
 * Author: Andy Bonilla
 *
 * Created on 26 de julio de 2021, 03:58 PM
 */

#ifndef UART_CONFIG_H
#define	UART_CONFIG_H
#define _XTAL_FREQ 4000000

#include <xc.h> // include processor files - each processor file is guarded.  

/*-----------------------------------------------------------------------------
----------- prototipo de funcion de configuracion de displays -----------------
-----------------------------------------------------------------------------*/
void uart_config(void);
void send_char (char dato);
void send_str(char st[]);
#endif	/* UART_CONFIG_H */