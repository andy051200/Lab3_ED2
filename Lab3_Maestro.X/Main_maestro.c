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
int cuenta1_timer1;         //control de mandar datos
/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //------
    if (PIR1bits.TMR1IF)
    {
        cuenta1_timer1++;
        PORTB++;
        TMR1H = 0;             // se reinicia el timer
        TMR1L = 0;             // se reinicia el timer
        PIR1bits.TMR1IF=0;
    }
 
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();
    
    
    while(1)
    {
        switch(cuenta1_timer1)
        {
            case(0):
                PORTCbits.RC2=0;    //se habilita control de asistente
                break;
                
            case(1):
                PORTB++;
                break;
                
            case(2):
                spiWrite(PORTB);    //se escribe el valor deseado a asistente
                break;
            
            case(3):
                PORTD=spiRead();    //se recibe el valor enviado de asistente
                break;
                
            case(4):                
                PORTCbits.RC2=1;    //se deshabilita control de asistente
                cuenta1_timer1=0;   //se reinicia la cuenta
                break;
            
        }   
    }
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //---------CONFIGURACION DE ENTRADAS ANALOGICAS
    ANSEL=0;                    //solo se limpian entradas analogicas
    ANSELH=0;                   //solo se limpian entradas analogicas
    
    //---------CONFIGURACION DE IN/OUT
    TRISB=0;                    //todo el portB como salida
    TRISCbits.TRISC2=0;         //salida de control de asistente o slave select
    TRISCbits.TRISC3=0;         //salida reloj control
    TRISCbits.TRISC4=1;         //salida para datos desde PIC maestro  
    TRISD=0;                    //todo el portB como salida
     
    //---------LIMPIEZA DE PUERTOS
    PORTB=0;
    PORTCbits.RC2=1;            //mantiene prendido el pin
    PORTD=0;
    
    //---------CONFIGURACION DEL TIMER1
    T1CONbits.T1CKPS1 = 1;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 1;   // bit 4
    T1CONbits.T1OSCEN = 1;   // Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // External Clock Input Synchronization Control bit
    T1CONbits.TMR1CS = 0;    // Clock Source Select bit..0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 0;             // preset for timer1 MSB register
    TMR1L = 0;             // preset for timer1 LSB register
    
   //---------LLAMADO DE FUNCIONES DESDE LIBRERIAS
    osc_config(4);          //Freq Osc a 4MGhz
    uart_config();          //configuracion de comunicacion UART
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    PIE1bits.RCIE=1;            //se habilita interrupcion de recepcion uart
    PIR1bits.RCIF=0;
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
