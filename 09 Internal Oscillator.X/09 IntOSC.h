// PICkit 2 Lesson file header

#ifndef PICKIT_H
#define PICKIT_H
#endif

/** D E C L A R A T I O N S **************************************************/
typedef enum { C_250kHz = 0,
               C_500kHz = 1,
               C_1MHz = 2,
               C_2MHz = 3,
               C_4MHz = 4,
               C_8MHz = 5,
               C_16MHz = 6,
               C_32MHz = 7,
               C_64MHz = 8} IntOSCFreq;

/** D E F I N I T I O N S ****************************************************/


/** E X T E R N S ************************************************************/
// declare variables accessible by other files.

/** P R O T O T Y P E S ******************************************************/
void interrupt InterruptService(void);
void SetupINT0Switch(void);
void EnableInterrupts(void);
void SetIntOSC(IntOSCFreq *ClockSet);
