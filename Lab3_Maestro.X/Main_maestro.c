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
#include "SPI_config.h"
//#include "UART_CONFIG.h"

/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);           //prototipo de funcion de inicializacion pic
void toggle_adc(void);
void mandar_datos(void);
/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
unsigned char cuenta1_timer0;
unsigned char cuenta2_timer0;
unsigned char cuenta_uart=0;
unsigned char uart_recibido1;
unsigned char uart_recibido2;
unsigned char map_pot1_cen;
unsigned char map_pot1_dec;
unsigned char map_pot2_cen;
unsigned char map_pot2_dec;
unsigned char uart_cen_pot1;
unsigned char uart_dec_pot1;
unsigned char uart_cen_pot2;
unsigned char uart_dec_pot2;


/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //------INTERRUPCION DEL TIMER1
    if (PIR1bits.TXIF)
    {
        cuenta_uart++;
        mandar_datos();     //invoco funcion para mandar uart
        PIR1bits.TXIF=0;
    }
    
    if (INTCONbits.T0IF)
    {
        cuenta1_timer0++;
        cuenta2_timer0++;
        INTCONbits.T0IF=0;
        TMR0 = 255;             // preset for timer register
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
        switch(cuenta1_timer0)
        {
            case(0):
                PORTCbits.RC2=0;
                break;
                
            case(2):
                spiWrite(1);                //le notifica que mande pot1
                uart_recibido1=spiRead();   //recibe pot1
                break;
            
            case(8):
                spiWrite(2);                //le notifica que mande pot2
                uart_recibido2=spiRead();   //recibe pot2
                break;
                
            case(10):
                PORTCbits.RC2=1;            //se pone en 1 el Slave select
                break;
        
            case(249):
                cuenta1_timer0=0;           //se espera el disque delay
                break;

        }
        //MAPEO DE POTENCIOMETRO 1
        map_pot1_cen=((2*(uart_recibido1)/100)%10);    //centenas de pot1
        map_pot1_dec=((2*(uart_recibido1)/10)%10);     //decenas de pot1
        
        //MAPEO DE POTENCIOMETRO 2
        map_pot2_cen=((2*(uart_recibido2)/100)%10);    //centenas de pot2
        map_pot2_dec=((2*(uart_recibido2)/10)%10);     //decenas de pot2
        
        //CONVERSION A ASCII POTENCIOMETRO 1
        uart_cen_pot1=(map_pot1_cen+0x30);  //se le suma 0x30 para ascii
        uart_dec_pot1=(map_pot1_dec+0x30);  //se le suma 0x30 para ascii
        
        //CONVERSION A ASCII POTENCIOMETRO 2
        uart_cen_pot2=(map_pot2_cen+0x30);  //se le suma 0x30 para ascii
        uart_dec_pot2=(map_pot2_dec+0x30);  //se le suma 0x30 para ascii
        
        
        PORTB=uart_recibido1;
        PORTD=uart_recibido2;

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
    
    //---------CONFIGURACION DE IN/OUT
    TRISB=0;                    //todo el portB como salida
    TRISCbits.TRISC2=0;         //salida de control de asistente o slave select
    TRISCbits.TRISC3=0;         //salida reloj control
    TRISCbits.TRISC4=1;         //salida para datos desde PIC maestro  
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=1;
    TRISD=0;                    //todo el portB como salida
    
    //---------LIMPIEZA DE PUERTOS
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
    osc_config(4);          //se llama funcion de oscilador a 4MHz
    //uart_config();           //se llama funcion de ADC
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //uart
    TXSTAbits.TX9 = 0; //TRANSMISION DE 8 BITS
    TXSTAbits.SYNC = 0; //ASINCRONO
    TXSTAbits.BRGH = 1; //HIGH SPEED
    BAUDCTLbits.BRG16 = 0; //BAUD RATE DE 8 BITS
    SPBRGH = 0;
    SPBRG = 25;
    PIE1bits.TXIE = 1;
    TXSTAbits.TXEN = 1;
    
    //CONFIG RX
    RCSTAbits.SPEN = 1;
    RCSTAbits. RX9 = 0;
    RCSTAbits.CREN = 1;
    
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    INTCONbits.PEIE=1;
    INTCONbits.T0IE=1;          //se habilita interrupcion timer 0
    INTCONbits.T0IF=0;          //se apaga bandera de interrupcion timer0
    PIE1bits.TXIE=1;
    PIE1bits.RCIE=1;
    PIR1bits.TXIF=0;
    PIR1bits.RCIF=0;
    /*PIE1bits.ADIE=1;
    PIR1bits.ADIF=0;*/
    /*PIE1bits.SSPIE = 1;         //se habilita interrupcion del MSSP
    PIR1bits.SSPIF = 0;         //se apaga bandera de interrupcion MSSP*/
    
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void mandar_datos(void)
{
    switch(cuenta_uart)
    {
        case(1):
            uart_cen_pot1;  //se mandan centenas en ascii de pot1
            break;
            
        case(2):
            0x2E;           //se manda el punto decimal
            break;
            
        case(3):
            uart_dec_pot1;  //se mandan decenas en ascii de pot1
            break;
            
        case(4):
            0x20;           //se deja espacio
            break;
            
        case(5):
            uart_cen_pot2;  //se mandan centenas en ascii de pot2
            break;
            
        case(6):
            0x2E;
            break;
            
        case(7):
            uart_dec_pot2;
            break;
            
        case(8):  
            0x0D;               //se da enter a los datos mandados
            cuenta_uart=0;
            break;
    }
    
}