#ifndef __CRC_32_H
#define __CRC_32_H

#include <stdlib.h>
#include <stdint.h>
#include "rcc.h"
//#include <arch/arm/stm32f10x_rcc.h>
/*!
 * \addtogroup CRC32
 */

/*@{*/
//#define CRC_BASE            (AHBPERIPH_BASE + 0x3000)

typedef struct
{
  volatile uint32_t DR;
  volatile uint8_t  IDR;
  uint8_t   RESERVED0;
  uint16_t  RESERVED1;
  volatile uint32_t CR;
} CRC_TypeDef;

#define CRC                 ((CRC_TypeDef *) CRC_BASE)

/*
void crc32_ena( void );
void crc32_reset( void );
void crc32_write_data( void * data, size_t size );
unsigned long crc32_read_data( void );
void crc_idr_write( unsigned char byte );
unsigned char crc_idr_read( void );
*/

unsigned long crc32( void * data, size_t size );


/*@}*/



#endif
