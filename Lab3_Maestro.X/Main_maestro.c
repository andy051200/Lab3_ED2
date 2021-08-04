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
unsigned char desde_interfaz;

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //------INTERRUPCION DEL TIMER1
    if (PIR1bits.TXIF)
    {
        cuenta_uart++;      //se suma variable guia
        mandar_datos();     //invoco funcion para mandar uart
        PIR1bits.TXIF=0;    //apago interrupcion
    }
    if (PIR1bits.RCIF)
    {
        desde_interfaz=RCREG;   //jalo dato que mando la interfaz
        PIR1bits.RCIF=0;        //apago bandera de interrupcion
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
        
        PORTCbits.RC2=0;
        __delay_ms(1);
        
        spiWrite(0b0001);                //le notifica que mande pot1
        uart_recibido1=spiRead();   //recibe pot1
        __delay_ms(1);
        PORTCbits.RC2=1;
        PORTCbits.RC2=0;
        __delay_ms(1);
        spiWrite(0b0010);                //le notifica que mande pot2
        uart_recibido2=spiRead();   //recibe pot2
        __delay_ms(1);
        PORTCbits.RC2=1;
        
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
        uart_dec_pot2=(map_pot2_dec+0x30);  //se le suma 0x30 para ascii*/
        
        //PEDAZO PARA SUMAR/RESTAR CON GUI
        if(desde_interfaz==0x31)
            PORTB++;
        else if (desde_interfaz==0x32)
            PORTB--;
        
        
        //PORTB=uart_recibido1;
        PORTB=uart_recibido2;
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
    TRISCbits.TRISC6=0;         //salida TX uart
    TRISCbits.TRISC7=1;         //entrada RX uart
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

    //---------CONFIGURACION DEL TIMER1
    T1CONbits.T1CKPS1 = 1;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 1;   // bit 4
    T1CONbits.T1OSCEN = 1;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 12;             // preset for timer1 MSB register
    TMR1L = 38;             // preset for timer1 LSB register

    //---------LLAMADO DE FUNCIONES DESDE LIBRERIAS
    osc_config(4);          //se llama funcion de oscilador a 4MHz
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //---------CONFIGURACION UART
    TXSTAbits.SYNC = 0;     //comunicacion asincrona
	TXSTAbits.BRGH = 1;     //comunicacion de alta velocidad
	TXSTAbits.TX9 = 0;      //comunicacion de 8bits
	BAUDCTLbits.BRG16 = 0;  //
	SPBRGH = 0;             //configuracion de braudeaje segun Freq osc
	SPBRG = 25;
	//SETUP RECIBIR
	RCSTAbits.SPEN = 1;     //se habilita modulo
	RCSTAbits.RX9 = 0;      //comunicacion de 8bits
	RCSTAbits.CREN = 0;     //
	TXSTAbits.TXEN = 1;     //
    
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    INTCONbits.PEIE=1;          //interrupcion por perifericos
    PIE1bits.TXIE=1;            //enable interrupcion de tx uart
    PIE1bits.RCIE=1;            //enable interrupcion de rx uart
    PIR1bits.TXIF=0;            //apago bandera interrupcion tx uart
    PIR1bits.RCIF=0;            //apago bandera interrupcion rx uart
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void mandar_datos(void)
{
    switch(cuenta_uart)
    {
        case 0:
            TXREG=0x20;
            break;
            
        case(1):
            TXREG=uart_cen_pot1;  //se mandan centenas en ascii de pot1
            break;
            
        case(2):
            TXREG=0x2E;           //se manda el punto decimal
            break;
            
        case(3):
            TXREG=uart_dec_pot1;  //se manda el punto decimal
            break;
            
        case(4):
            TXREG=0x20;           //se deja espacio
            break;
            
        case(5):
            TXREG=0x20;           //se deja espacio
            break;
            
        case(6):
            TXREG=uart_cen_pot2;  //se mandan centenas en ascii de pot2
            break;
            
        case(7):
            TXREG=0x2E;           //se manda el punto decimal
            break;
            
        case(8):
            TXREG=uart_cen_pot2;
            break;
            
        case(9):  
            TXREG=13;               //se da enter a los datos mandados
            break;
            
        case(200):
            TXREG=cuenta_uart;
            cuenta_uart=0;
            break;
    }
    
}