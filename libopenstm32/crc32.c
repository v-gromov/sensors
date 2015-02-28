#include "crc32.h" 

/*!
 * \addtogroup CRC32
 */

/*@{*/


#define CRC32_RESET_BIT 1 
#define IDLE_MASK 0x00000000


/** \brief Enables CRC32 module in stm32 
 */

static void crc32_ena( void )
{
  rcc_peripheral_enable_clock(&RCC_AHBENR,CRCEN);
}

/** \brief Sets CRC32 data register to 0xFFFFFFFF 
 */
static void crc32_reset( void )
{
  CRC->CR = CRC32_RESET_BIT;
}

/** \brief Sends data to CRC32 data register
 *  \param data pointer to data
 *  \size size of sent data  
 */
static void crc32_data_write( void * data, size_t size )
{
  unsigned long * word32 = (unsigned long*)data;

  while( size > 0 ) {
    if (size > sizeof(unsigned long) ) {
      CRC->DR = *word32++;
      size -= sizeof(unsigned long);
    } else {
      CRC->DR = IDLE_MASK | ( *word32
                             << (sizeof(unsigned long) - size) * 8 );
      size = 0;
    }
  }
}

/** \brief Reads result of CRC32 calculation from CRC32 data register
 *  \return CRC32 value  
 */
static unsigned long crc32_data_read( void )
{
  return CRC->DR;
}

/** \brief Calculates CRC32 for given data
 *  \param data pointer to data
 *  \size size of sent data  
 *  \return CRC32 value  
 */
unsigned long crc32( void * data, size_t size )
{
  crc32_ena( );
  crc32_reset( );

  crc32_data_write( data, size );

  return crc32_data_read( );
}

/*
void crc_idr_write( unsigned char byte )
{
  CRC->IDR = byte;
} 

unsigned char crc_idr_read( void )
{
  return CRC->IDR;
}
*/

/*@}*/


