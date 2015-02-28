#include <stdlib.h>
#include "spi.h"

/** \file dataflash.c
 *  
 * \brief  AT25 DataFlash supporting routines.
 *
 *  B5_MOD_MC has AT25DF641 flash installed.
 * \todo add documentation
 */

/** \addtogroup DataFlash
  * \{
  */

/*@{*/

/** \brief SPI bus on which dataflash resides
  */
#define DFSPI  SPI3

/** \brief initialise dataflash
  * 
  * As the dataflash does not require initialisation, so just initialise SPI bus
  * on which the dataflash is located
  */
void dataflash_init( )
{
  spi3_setup();
}

/** \brief send one byte to the dataflash and ignore returned byte
  * \param b -- byte to send
  * \sa DFSPI
  * \sa spi_transfer()
  */
static void send_byte( unsigned char b )
{
  unsigned char d = b;
  spi_transfer( DFSPI, 1, &d, NULL );
}

/** \brief send address to the dataflash chip
  *
  * Sends 3 bytes of address to the dataflash in MSB-fist manner
  * \param address -- address to send to the dataflash chip
  */
static void send_address( unsigned long address )
{
  unsigned char addr[3];
  addr[2] = address & 0xfful;
  address >>=8;

  addr[1] = address & 0xfful;
  address>>=8;

  addr[0] = address & 0xfful;
  spi_transfer( DFSPI, 3, addr, NULL );
};

/** \brief Enable writing to dataflash
  * \note that WEL bit in dataflash is dropped after executing an operation requiring it being set.
 */
void dataflash_we( unsigned char state )
{
  unsigned char data;
  if( state ) {
    data = 0x06;
  }
  else {
    data = 0x04;
  };

  spi3_transfer( 1, &data, NULL );
};

/** \brief Returns the business flag of dataflash.
 *
 *  \return 1 means that erase or write operation is in progress.          
 */
unsigned char dataflash_busy( void )
{
  unsigned char d[3];
  d[0] = 0x05;

  spi3_transfer( 3, d, d);
  
  return d[1] & ( 1 << 0 );
};

/** \brief Check wherever dataflash is write enabled
  * \return 0 if DF is write protected, non zero otherwise.
  * \sa dataflash_we()
  */
unsigned char dataflash_write_enabled( void )
{
  unsigned char d[2];
  d[0] = 0x05;

  spi3_transfer( 2, d, d );

  return d[1] & ( 1 << 1 );
};

/** \brief reads sector protection status
  * 
  * \param address -- Address of required sector. Lower 16 bits are ignored.
  * \return Returns 0 if sector is unprotected, or 0xff otherwise.
  */
unsigned char dataflash_read_sector_lockdown( unsigned long address )
{
  spi3_slave_select( 1 );
  send_byte( 0x3c );
  send_address( address );
  send_byte( 0xff ); // Don't care byte

  unsigned char lock;
  spi_transfer( DFSPI, 1, NULL, &lock );
  spi3_slave_select( 0 );

  return lock;
};

/** \brief Unlock dataflash sector (64kb).
  *
  * Unlocks dataflash sector 64Kb.
  * \param address -- Address of sector to unlock. Lower 16 bits are ignored.
  * \note dataflash_we(1) must be called before this function.
  */
void dataflash_sector_unlock( unsigned long address )
{
  spi3_slave_select( 1 );
  send_byte( 0x39 );
  send_address( address );
  spi3_slave_select( 0 );
};

/** \brief Unlock all the sectors of dataflash
 *
 *  Funtion writes to Status Register Byte 1
 *  0x00 data, that means unprotection of entire chip.
 *
 *  \note dataflash_we(1) must be called before this function.
 */
void dataflash_global_unlock( )
{
  spi3_slave_select( 1 );
  send_byte( 0x01 ); 
  send_byte( 0x00 );
  spi3_slave_select( 0 );
}

/** \brief Lock all the sectors of dataflash
 *
 * Function performes reverse operation to 'global unlock'
 *
 * \sa dataflash_global_unlock()
 */
void dataflash_global_lock( )
{
  spi3_slave_select( 1 );
  send_byte( 0x01 ); 
  send_byte( 0x3C ); // this means protect: 0X1111XX
  spi3_slave_select( 0 );
}

/** \brief erases whole dataflash chip
  * \warning takes up to 2 minutes, use with caution
  */
void dataflash_chip_erase( void )
{
  unsigned char data = 0x60;

  dataflash_we( 1 );
  dataflash_global_unlock();

  dataflash_we( 1 );
  spi3_transfer( 1, &data, NULL );

  while( dataflash_busy(  ) );

  dataflash_we( 1 );
  dataflash_global_lock();
};

/** \brief Erases 4k block in dataflash
  *
  * \param address -- block to erase. Lower 12 bits are ignored.
  */
void dataflash_block_4k_erase( unsigned long address )
{
  dataflash_we( 1 );
  
  spi3_slave_select( 1 );
  send_byte( 0x20 );
  send_address( address );
  spi3_slave_select( 0 );

  while( dataflash_busy() );
};


/** \brief Erases 64k block in dataflash
  *
  * \param address -- block to erase. Lower 12 bits are ignored.
  */
void dataflash_block_64k_erase( unsigned long address )
{
  dataflash_we( 1 );

  spi3_slave_select( 1 );
  send_byte( 0xD8 );
  send_address( address );
  spi3_slave_select( 0 );

  while( dataflash_busy() );
};

/** \brief Write up to a page from dataflash
  *
  * Maximum write size is 256 bytes. 
  * For correct results, you can only write to the addresses containing 0xff.
  * To guarantee this you should perform a block erase
  * \sa dataflash_block_4k_erase
  * \param address -- address for writing
  * \param buffer -- data to write
  * \param size -- write data length
  * 
  */
void dataflash_block_write( unsigned long address, unsigned char * buffer,
                            unsigned short size )
{
  if( dataflash_read_sector_lockdown( address ) ) {
    dataflash_we( 1 );
    dataflash_sector_unlock( address );
  };

  dataflash_we( 1 );
  spi3_slave_select( 1 );
  send_byte( 0x02 );
  send_address( address );
  spi_transfer( DFSPI, size, buffer, NULL );
  spi3_slave_select( 0 );

  while( dataflash_busy(  ) ) {
  }
};

/** \brief Read up to a page from dataflash
  *
  * Maximum read size is 256 bytes
  * \param address -- address for reading
  * \param buffer -- buffer for storing read data
  * \param size -- how much to read
  * 
  */
void dataflash_block_read( unsigned long address, unsigned char * buffer,
                           unsigned short size )
{
  spi3_slave_select( 1 );
  send_byte( 0x0b );
  send_address( address );
  send_byte( 0xff ); // don't care byte
  spi_transfer( DFSPI, size, NULL, buffer);
  spi3_slave_select( 0 );
};

/*@}*/

