/** \file fpgaregs.c
  * \brief Access functions for FPGA registers
  */
#include <stdio.h>

#include <common/service.h>
//#include <common/usart2.h>
#include <common/fpgaregs.h>

#include "fpgaregs.h"

/** \addtogroup FPGA_REGS
  * \{
  */


/** \brief Initialises FPGA registers.
  * 
  * Selects port 0 as an ADC source. Enables SSCOPE and PCM codec
  */
void fpgaregs_init( void )
{
  //Set adc port to 0
  fpgaregs_write( FPGA_ADCCR2, 0 );
  //Enable pcm codec and sscope
  fpgaregs_write( FPGA_SECR, FPGA_SECR_SSC | FPGA_SECR_PCMC );
};

/** \brief Checks if FPGA registers are accessible
  * \return 0 in case of successfull read and write, -1 otherwise
  */
int fpga_registers( void )
{
  int i = 0x5a;
  fpgaregs_write( 0x03, i );
  if( fpgaregs_read( 0x03 ) != i )
    return -1;
  return 0;
};

/** \sa command_rw_callback()
  */
static int address_check( int address )
{
  if( address < 0 )
    return -1;
  if( address > 65535 )
    return -1;
  return 0;
};

/** \sa command_rw_callback()
  */
static int data_check( int data )
{
  if( data < 0 )
    return -1;
  if( data > 65535 )
    return -1;
  return 0;
};

/** \brief Read or write fpga registers
 *
 * Read or write fpga registers.
 *  Read/write commands are received from usart2 in format 
 *  described in parse_address_data() function.
 * 
 *  \sa parse_address_data()
 *  \sa command_rw_callback()
 */
int fpgaregs_rw( void )
{
  return command_rw_callback( fpgaregs_read, fpgaregs_write,
                              address_check, data_check );
}

/**
  * \}
  */
