/*
 * File:   main.c
 * Author: 664077
 *
 * Created on November 5, 2025, 9:49 AM
 */

//Configuration - just what's needed
#pragma config FOSC = INTIO67       // Sets use of the Internal oscillator of PIC18FK22, "67" relates to setting crystal (OSC1) to I/O not for clock input.
#pragma config WDTEN = OFF          // Watchdog off, this is done because it will automatically disable flash code after a crash.
#pragma config LVP = ON             // Low voltage programming, 5V
#pragma config PBADEN = OFF         // PORTB digital

#include <xc.h>
#define _XTAL_FREQ 1000000

void main(void) {
    OSCCON = 0x30;      // 1 MHz
    ANSELB = 0;         // Port B Digital
    TRISB = 0;          // Port B Output
    
    while(1) {
        LATB = 0xFF;    // All ON
        __delay_ms(250);
        LATB = 0x00;    // All OFF
        __delay_ms(250);
    }
}



