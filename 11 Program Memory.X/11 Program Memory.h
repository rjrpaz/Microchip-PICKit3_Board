// PICkit 2 Lesson file header

#ifndef PICKIT_H
#define PICKIT_H
#endif

/** D E C L A R A T I O N S **************************************************/


/** D E F I N I T I O N S ****************************************************/

/** E X T E R N S ************************************************************/
// declare variables accessible by other files.

/** P R O T O T Y P E S ******************************************************/
unsigned char ProgMemRdAddress(unsigned int address);
// unsigned char ProgMemErase64(unsigned int address);
void ProgMemErase64(unsigned int address);
// unsigned char ProgMemWr32(unsigned int address, unsigned char *buffer_ptr);
void ProgMemWr32(unsigned int address, unsigned char *buffer_ptr);
