//******************************************************************************
//Software License Agreement                                         
//                                                                    
//The software supplied herewith by Microchip Technology             
//Incorporated (the "Company") is intended and supplied to you, the  
//Company's customer, for use solely and exclusively on Microchip
//products. The software is owned by the Company and/or its supplier,
//and is protected under applicable copyright laws. All rights are   
//reserved. Any use in violation of the foregoing restrictions may   
//subject the user to criminal sanctions under applicable laws, as   
//well as to civil liability for the breach of the terms and         
//conditions of this license.                                        
//                                                                    
//THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,  
//WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED  
//TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A       
//PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,  
//IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR         
//CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.       
// *******************************************************************
// PIC18F45K20 PICkit 3 Debug Express Lesson 12 - CCP PWM
//
// This lesson demonstrates using the ECCP1 module to create a PWM
// output signal tp LED 7.  The PWM signal is modulated, meaning the
// duty cycle is changed, to change the brightness of the LED.
//
// *******************************************************************
// *    See included documentation for Lesson instructions           *
// *******************************************************************

/** C O N F I G U R A T I O N   B I T S ******************************/

#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                      // CONFIG1H
#pragma config PWRT = OFF, BOREN = OFF, BORV = 30                           // CONFIG2L
#pragma config WDTEN = OFF, WDTPS = 32768                                   // CONFIG2H
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC      // CONFIG3H
#pragma config STVREN = ON, LVP = OFF, XINST = OFF                          // CONFIG4L
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF                   // CONFIG5L
#pragma config CPB = OFF, CPD = OFF                                         // CONFIG5H
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF               // CONFIG6L
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF                           // CONFIG6H
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF           // CONFIG7L
#pragma config EBTRB = OFF                                                  // CONFIG7H

/** I N C L U D E S **************************************************/
#include <xc.h>
#include <plib/delays.h>
#include "12 CCP PWM.h"  // header file

/** V A R I A B L E S *************************************************/
// #pragma idata   // declare statically allocated initialized variables


/** D E C L A R A T I O N S *******************************************/
// #pragma code  // declare executable instructions

void main (void)
{
    unsigned char brightness = 125; // = 0x7D

    // Set RD7/P1D pin output so P1D PWM output drives LED7
    TRISDbits.TRISD7 = 0;

    // Set up 8-bit Timer2 to generate the PWM period (frequency)
    T2CON = 0b00000111;// Prescale = 1:16, timer on, postscale not used with CCP module
    PR2 = 249;         // Timer 2 Period Register = 250 counts
    // Thus, the PWM frequency is:
    // 1MHz clock / 4 = 250kHz instruction rate.
    // (250kHz / 16 prescale) / 250) = 62.5Hz, a period of 16ms.

    // The Duty Cycle is controlled by the ten-bit CCPR1L<7,0>:DC1B1:DC1B0
    // 50% Duty Cycle = 0.5 * (250 * 4) = 500
    CCPR1L = 0x7D;   // The 8 most significant bits of the value 500 = 0x1F4 are 0x7D
                     // The 2 least significant bits of the value (0b00) are written
                     // to the DC1B1 and DC1B0 bits in CCP1CON
    CCP1CON = 0b01001100;
                     // P1Mx = 01 Full-Bridge output forward, so we get the PWM
                     // signal on P1D to LED7.  Only Single Output (00) is needed,
                     // but the P1A pin does not connect to a demo board LED
                     // CCP1Mx = 1100, PWM mode with P1D active-high.

    // The LED brightness is affected by by the Duty Cycle, which determines how much
    // of each 16ms period it is on and how much it is off.  As the duty cycle gets
    // less than 50%, it is off more than it is on so the LED becomes dimmer.  As the 
    //duty cycle increases above 50%, it is on more than off, so it gets brighter.
    //
    // This increases the brightness over 2 seconds, then decreases it over the next 2 seconds
    // Updating the CCPR1L value more than once per 16ms period has no benefit, so we'll update
    // it a total of 125 times, once per period, which works out to 2 seconds.
    //
    // Although we have nearly ten bits of resolution in the duty cycle (1000 counts)
    // we'll increment the duty cycle by 8 each time as we only have 125 levels over the
    // 2 second period.
    while(1)
    {
        // increase brightness over 2 seconds.
        do
        {
            brightness += 2;
            CCPR1L = brightness;    // + 8 including 2 bits DC1Bx in CCP1CON
            PIR1bits.TMR2IF = 0;        // clear interrupt flag; set on every TMR2 = PR2 match
            while (PIR1bits.TMR2IF == 0); // watch for match, which is end of period.
        } while (brightness < 250);

        Delay1KTCYx(63);    // delay about 250ms at peak brightness, just for effect!

        // decrease brightness over 2 seconds.
        do
        {
            brightness -= 2;
            CCPR1L = brightness;    // - 8 including 2 bits DC1Bx in CCP1CON
            PIR1bits.TMR2IF = 0;        // clear interrupt flag; set on every TMR2 = PR2 match
            while (PIR1bits.TMR2IF == 0); // watch for match, which is end of period.
        } while (brightness > 1);

        Delay1KTCYx(63);    // delay about 250ms at dimmest, it gives a better effect!
    };    	
}

