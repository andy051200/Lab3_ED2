/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: laboratorio 3
Hardware: PIC16F887
    
Creado: 30 de julio de 2021    
Descripcion: un laboratoria bien fumado tbh pero chilero
------------------------------------------------------------------------------*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT   //configuracion de oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // master clear off 
#pragma config CP = OFF         // Code Protection bit off
#pragma config CPD = OFF        // Data Code Protection bit off
#pragma config BOREN = OFF      // Brown Out Reset Selection bits off
#pragma config IESO = OFF       // Internal External Switchover bit off
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit off 
#pragma config LVP = OFF        // Low Voltage Programming Enable bit off 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <xc.h>                 //se incluye libreria del compilador
#include <stdint.h>             //se incluye libreria
#include <pic16f887.h>          //se incluye libreria del pic
#include "Osc_config.h"
#include "ADC_CONFIG.h"
#include "UART_CONFIG.h"
#include "SPI_config.h"
/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);           //prototipo de funcion de inicializacion pic

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //------
    
 
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();
    
    
    while(1)
    {
        
    }

}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //---------CONFIGURACION DE ENTRADAS ANALOGICAS
    ANSEL=0;                //solo se limpian entradas analogicas
    ANSELH=0;               //solo se limpian entradas analogicas
    ANSELbits.ANS0=1;       //entrada para el potenciometro 1
    ANSELbits.ANS1=1;       //entrada para el potenciometro 2
    
    //---------CONFIGURACION DE IN/OUT
    TRISB=0;             //todo el portB como salida
    TRISCbits.TRISC6=0;     //salida TX
    TRISCbits.TRISC7=1;
    //TRISD=0;             //todo el portB como salida
    TRISDbits.TRISD5=0;     //salida para pines lcd
    TRISDbits.TRISD6=0;     //salida para pines lcd
    TRISDbits.TRISD7=0;     //salida para pines lcd
    
    //---------LIMPIEZA DE PUERTOS
    PORTB=0;
    PORTD=0;
    
   //---------LLAMADO DE FUNCIONES DESDE LIBRERIAS
    osc_config(4);          //Freq Osc a 4MGhz
    ADC_config();           //configuracion de ADC
    uart_config();          //configuracion de comunicacion UART
    spi_config();           //configuracino de comunicacion SPI
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    PIE1bits.RCIE=1;            //se habilita interrupcion de recepcion uart
    PIR1bits.RCIF=0;
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
