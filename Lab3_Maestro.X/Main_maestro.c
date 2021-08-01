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
unsigned char cuenta1_timer0;         //control de mandar datos
/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //------INTERRUPCION DEL TIMER1, CADA 0.5sec
    if (INTCONbits.T0IF==1)
    {
        cuenta1_timer0++;
        //PORTB++;
        INTCONbits.T0IF=0;
        TMR0 = 255; 
        
        /*if (cuenta1_timer1==4)
        {
            PORTD=~PORTD;
            cuenta1_timer1=0;
        }*/
        
       
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
        switch(cuenta1_timer0)
        {
            case(0):
                PORTCbits.RC2=0;
                break;
                
            case(2):
                spiWrite(PORTB);
                PORTD=spiRead();
                //PORTD=~PORTD;
                break;
                
            case(4):
                PORTCbits.RC2=1;
                break;
        
            case(249):
                PORTB++;
                cuenta1_timer0=0;
                break;

        }
        if (PORTB==0xff)
            PORTB=0;
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
    
    //---------CONFIGURACION DEL TIMER0
    OPTION_REGbits.T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
    OPTION_REGbits.PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
    OPTION_REGbits.PS2 = 1;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 255;             // preset for timer register

    
   //---------LLAMADO DE FUNCIONES DESDE LIBRERIAS
    osc_config(4);          //Freq Osc a 4MGhz
    uart_config();          //configuracion de comunicacion UART
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    PIE1bits.TMR1IE=1;          //se habilita interrupcion del timer1
    INTCONbits.T0IE=1;
    INTCONbits.T0IF=0;
    //PIE1bits.RCIE=1;            //se habilita interrupcion de recepcion uart
    //PIR1bits.RCIF=0;            //se apaga bandera de interrupcion uart
    PIR1bits.TMR1IF=0;          //se apaga bandera de interrupcion timer1
    return;
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/