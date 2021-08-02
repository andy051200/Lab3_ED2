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
#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);           //prototipo de funcion de inicializacion pic
void toggle_adc(void);
/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
unsigned char cuenta1_timer0;
unsigned char cuenta2_timer0;
unsigned char conversion1;
unsigned char conversion2;

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //------INTERRUPCION DEL TIMER1
    if (INTCONbits.T0IF)
    {
        cuenta1_timer0++;
        cuenta2_timer0++;
        INTCONbits.T0IF=0;
        TMR0 = 255;             // preset for timer register
    }
    
    //------INTERRUPCION RECEPCION DE DATOS DESDE PIC MAESTRO
    if (PIR1bits.SSPIF)
    {
        PORTD=spiRead();        //se lee valor proveniente del PIC maestro
        spiWrite(conversion1);        //se envia el valor hacia el PIC maestro
        
        PIR1bits.SSPIF=0;       //se apaga bandera de interrupcion
    }
    
    //------INTERRUPCION 
    if (PIR1bits.ADIF)
    {
        if (ADCON0bits.GO==0)
        {
            switch(ADCON0bits.CHS)
            {
                case(0):
                    conversion1=ADRESH; //conversion del pot1
                    ADCON0bits.CHS=1;   //cambio de canal
                    break;
                    
                case(1):
                    conversion2=ADRESH; //conversion del pot1
                    ADCON0bits.CHS=0;   //cambio de canal
                    break;
            }
        }
        __delay_us(50);
        ADCON0bits.GO=1;
        PIR1bits.ADIF=0;
    }
    
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();        //se llama funcion de configuracion
    __delay_us(100);
    ADCON0bits.GO=1;
    while(1)
    {
        PORTB=conversion1;
        /*PORTD=conversion2;*/
        /*switch(cuenta1_timer0)
        {    
            case(249):
                PORTB++;
                break;
                
            case(250):
                cuenta1_timer0=0;
                break;
        }*/
        
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
    TRISAbits.TRISA0=1;
    TRISAbits.TRISA1=1;
    TRISAbits.TRISA5=1;     //entrada control para datos desde PIC maestro
    TRISB=0;                //todo el portB como salida
    TRISD=0;                //todo el PortD como salida
    
    //---------LIMPIEZA DE PUERTOS
    PORTB=0;
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
    osc_config(4);          //se llama funcion de oscilador a 4MHz
    ADC_config();           //se llama funcion de ADC
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    INTCONbits.T0IE=1;          //se habilita interrupcion timer 0
    INTCONbits.T0IF=0;          //se apaga bandera de interrupcion timer0
    PIE1bits.ADIE=1;
    PIR1bits.ADIF=0;
    PIE1bits.SSPIE = 1;         //se habilita interrupcion del MSSP
    PIR1bits.SSPIF = 0;         //se apaga bandera de interrupcion MSSP
    
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
/*void toggle_adc(void)
{
    if (ADCON0bits.GO==0)
    {
        switch(ADCON0bits.CHS)
        {
            case(0):
                PORTB=ADRESH;         //potenciometro 1
                __delay_us(500);            //delay para cargar capacitor          
                ADCON0bits.CHS=1;           //switch de canal
                ADCON0bits.GO=1;            //se inicia otra conversion ADC
                break;
                    
            case(1):
                PORTD=ADRESH;         //potenciometro 2
                __delay_us(500);            //delay para cargar capacitor
                ADCON0bits.CHS=0;           //switch de canal
                ADCON0bits.GO=1;            //se inicia otra conversion ADC
                break;
        }
    }
}*/