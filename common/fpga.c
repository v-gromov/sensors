/** \file fpga.c
  * \brief This file contains functions for uploading FPGA firmware
  */

#include <common/io.h>
#include "spi.h"
#include "fpga.h"

/** \addtogroup FPGA_CONF
  * \{
  */

/*@{*/
void fpga_reset(void)
{
  b5_set( CARD_NCONFIG ); // FPGA nCONFIG = 1
  b5_clear( CARD_NCONFIG ); // FPGA nCONFIG = 0
}

int fpga_begin_conf(void)
{
  b5_clear( CARD_NCONFIG ); // FPGA nCONFIG = 0
  b5_set( CARD_NCONFIG ); // FPGA nCONFIG = 1
  //Wait for nSTATUS indicating that FPGA is ready to receive configuration
  while( !b5_get( CARD_NSTATUS )) continue;
  return 0;//All is ok, really. I mean really. ;-)
}

/** Send block to FPGA via passive serial interface.
 *  Checks for block error after sending. Checking is based
 *  on the nSTATUS and CRC_ERROR pins.
 * 
 * \return -EBADBLOCK if block write error occured,
 *         otherwise returns 0.
 */
int fpga_send_ps(uint8_t* block,int count)
{
  spi_transfer( SPI2, count, block, 0 );
  if ( !b5_get( CARD_NSTATUS ) ) { //nSTATUS indicates error
      return -EBADBLOCK; 
  }
  if ( b5_get( CARD_CRC_ERR ) ){//CRC_ERR
      return -EBADBLOCK;
  };
  return 0;
};

int fpga_confdone( void )
{
  return b5_get( CARD_CONF_DONE ); //CONF_DONE
};

/*@}*/
