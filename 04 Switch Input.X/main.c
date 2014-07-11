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
// PICkit 2 PIC18F4520 Lesson 4 - Switch Inputs
//
// This lesson shows one method to software debounce a switch input.
// The switch is sampled once every millisecond and must be low for
// 5 samples in a row to register a "press".  Each press rotates
// the LEDs on PORTD.
//
// *******************************************************************
// *    See included documentation for Lesson instructions           *
// *******************************************************************

/** C O N F I G U R A T I O N   B I T S ******************************/

#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                       // CONFIG1H
#pragma config PWRT = OFF, BOREN = SBORDIS, BORV = 30                        // CONFIG2L
#pragma config WDTEN = OFF, WDTPS = 32768                                     // CONFIG2H
#pragma config MCLRE = OFF, LPT1OSC = OFF, PBADEN = ON, CCP2MX = PORTC       // CONFIG3H
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
#include "04 Switch Input.h"  // header file

/** V A R I A B L E S *************************************************/
// #pragma udata   // declare statically allocated uinitialized variables
unsigned char LED_Display;  // 8-bit variable

/** D E C L A R A T I O N S *******************************************/
// #pragma code    // declare executable instructions

void main (void)
{
    unsigned char Switch_Count = 0;

    LED_Display = 1;            // initialize

    TRISD = 0b00000000;     	// PORTD bits 7:0 are all outputs (0)
    INTCON2bits.RBPU = 0;	// enable PORTB internal pullups
    WPUBbits.WPUB0 = 1;		// enable pull up on RB0
    ANSELH = 0x00;              // AN8-12 are digital inputs (AN12 on RB0)
    TRISBbits.TRISB0 = 1;       // PORTB bit 0 (connected to switch) is input (1)

    while (1)
    {
        LATD = LED_Display;     // output LED_Display value to PORTD LEDs

        LED_Display <<= 1;      // rotate display by 1

        if (LED_Display == 0)
            LED_Display = 1;    // rotated bit out, so set bit 0


        while (Switch_Pin != 1);// wait for switch to be released

        Switch_Count = 5;
        do
        { // monitor switch input for 5 lows in a row to debounce
            if (Switch_Pin == 0)
            { // pressed state detected
                Switch_Count++;
            }
            else
            {
                Switch_Count = 0;
            }   
            Delay10TCYx(25);    // delay 250 cycles or 1ms.
        } while (Switch_Count < DetectsInARow); 
    }
	
}