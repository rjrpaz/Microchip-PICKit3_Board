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
// PIC18F45K20 PICkit 3 Debug Express Lesson 10 - Using EEPROM
//
// This lesson demonstrates reading and writing of the internal
// 256 bytes of EEPROM in the PIC18F45K20.
//
// *******************************************************************
// *    See included documentation for Lesson instructions           *
// *******************************************************************

/** C O N F I G U R A T I O N   B I T S ******************************/

#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                      // CONFIG1H
#pragma config PWRT = OFF, BOREN = OFF, BORV = 30                       // CONFIG2L
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
#include "10 EEPROM.h"  // header file

/** V A R I A B L E S *************************************************/
// #pragma idata   // declare statically allocated initialized variables


/** D E C L A R A T I O N S *******************************************/
// #pragma code    // declare executable instructions

void main (void)
{
    unsigned char ee_address = 0; // init to first location.
    unsigned char number_count = 255; 

    // Init I/O
    TRISD = 0b00000000;     	// PORTD bits 7:0 are all outputs (0)
    TRISEbits.TRISE0 = 1;		// TRISE0 input

    INTCON2bits.RBPU = 0;		// enable PORTB internal pullups
    WPUBbits.WPUB0 = 1;			// enable pull up on RB0

    do
    { // Write numbers 0 to 255 from the last EEPROM location (address = 255)
      // to the first (address = 0)

        // put the number_count value on the LEDs.  When all locations are written,
        // all LEDs will be on.  If there is an error, the LEDs will have the value
        // of nuber_count on which the error occurs.
        LATD = number_count;

        EEPROM_Write(ee_address, number_count); // write data number_count to location ee_address

        // verify the write
        if (number_count != EEPROM_Read(ee_address))
        {
            break;      // get out of the loop
        }
        
        ee_address++;   // next address location
        number_count--; // next lower number value

    } while (number_count > 0);  // number_count will roll over from 0xFF to 0x00
                                 // after 256 bytes have been written.

    while(1)
    {
    };      // all done or error occured
	
}

void EEPROM_Write(unsigned char address, unsigned char databyte)
{ // writes the "databyte" value to EEPROM at the address given
  // location in "address".
    EECON1bits.EEPGD = 0;   // Set to access EEPROM memory
    EECON1bits.CFGS = 0;    // Do not access Config registers

    EEDATA = databyte;      // Load EEDATA with byte to be written
    EEADR = address;        // Load EEADR with address of location to write.

    EECON1bits.WREN = 1;    // Enable writing
    
    // NOTE: as a protective measure to prevent accidental writes, the following
    // sequence must be completed without interruption to write a byte to EEPROM.
    // Therefore, if interrupts are used they should be disabled during the sequence
    // as it would not work if an interrupt was received during the sequence.

    //INTCONbits.GIE = 0;   // Disable interrupts
    EECON2 = 0x55;          // Begin Write sequence
    EECON2 = 0xAA;
    EECON1bits.WR = 1;      // Set WR bit to begin EEPROM write
    //INTCONbits.GIE = 1;   // re-enable interrupts
    
    while (EECON1bits.WR == 1)
    {   // wait for write to complete.  WR bit will be cleared when write finishes
        // EEPROM write completion will also set the EEIF flag bit in PIR2, which
        // may be used to generate an interrupt when the EEPROM write completes.
    };

    EECON1bits.WREN = 0;    // Disable writing as a precaution.
}

unsigned char EEPROM_Read(unsigned char address)
{ // reads and returns the EEPROM byte value at the address given
  // given in "address".

    EECON1bits.EEPGD = 0;   // Set to access EEPROM memory
    EECON1bits.CFGS = 0;    // Do not access Config registers

    EEADR = address;        // Load EEADR with address of location to write.

    // execute the read
    EECON1bits.RD = 1;      // Set the RD bit to execute the EEPROM read

    // The value read is ready the next instruction cycle in EEDATA.  No wait is
    // needed.

    return EEDATA;
}
