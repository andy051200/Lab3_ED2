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
#include "SPI_config.h"

/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
//#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);           //prototipo de funcion de inicializacion pic

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
unsigned char cuenta1_timer1;

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //------INTERRUPCION DEL TIMER1
    if (PIR1bits.TMR1IF)
    {
        cuenta1_timer1++;
        PIR1bits.TMR1IF=0;
    }
    
    //------INTERRUPCION RECEPCION DE DATOS DESDE PIC MAESTRO
    if (PIR1bits.SSPIF)
    {
        PORTD=spiRead();        //se lee valor proveniente del PIC maestro
        spiWrite(PORTB);        //se envia el valor hacia el PIC maestro
        PIR1bits.SSPIF=0;       //se apaga bandera de interrupcion
    }
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();        //se llama funcion de configuracion
    
    
    while(1)
    {
        switch(cuenta1_timer1)
        {
            case(1):
                PORTB++;
                break;
                
            case(2):
                cuenta1_timer1=0;
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
    ANSEL=0;                //solo se limpian entradas analogicas
    ANSELH=0;               //solo se limpian entradas analogicas
    ANSELbits.ANS0=1;       //entrada para el potenciometro 1
    ANSELbits.ANS1=1;       //entrada para el potenciometro 2
    
    //---------CONFIGURACION DE IN/OUT
    TRISAbits.TRISA5=1;     //entrada control para datos desde PIC maestro
    TRISB=0;                //todo el portB como salida
    TRISD=0;                //todo el PortD como salida
    
    //---------LIMPIEZA DE PUERTOS
    PORTB=0;
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
    osc_config(4);          //se llama funcion de oscilador a 4MHz
    ADC_config();           //se llama funcion de ADC
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    PIE1bits.TMR1IE=1;         //se habilita interrupcion del timer1
    PIR1bits.TMR1IF=0;          //se apaga bandera de interrupcion timer1
    PIE1bits.SSPIE = 1;         //se habilita interrupcion del MSSP
    PIR1bits.SSPIF = 0;         //se apaga bandera de interrupcion MSSP
    
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
