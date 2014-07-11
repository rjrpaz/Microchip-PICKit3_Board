// PICkit 2 Lesson file header

#ifndef PICKIT_H
#define PICKIT_H
#endif

/** D E C L A R A T I O N S **************************************************/
typedef enum { LEFT2RIGHT,
               RIGHT2LEFT} LEDDirections;

// typedef enum {FALSE, TRUE} BOOL;


/** D E F I N I T I O N S ****************************************************/
#define Switch_Pin      PORTBbits.RB0
#define DetectsInARow   5

/** E X T E R N S ************************************************************/
// declare variables accessible by other files.

/** P R O T O T Y P E S ******************************************************/
void Timer0_Init(void);
void ADC_Init(void);
unsigned char ADC_Convert(void);

