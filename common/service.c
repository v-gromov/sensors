#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "dataflash.h"
#include "fpga.h"
#include "service.h"
#include <common/io.h>
#include <common/fpgaregs.h>

/** \addtogroup SERVICE 
  * \{
  */

/*@{*/

/** Transfer firmware from dataflash to fpga via passive serial.
 *  Firmware is stored at DF_FIRMWARE_OFFSET address and it's 
 *  maximum size does not mean because fpga notifies us via 
 *  nSTATUS pin about incorrect firmware.
 *
 * \return -1 on error, 0 otherwise
 */

int df2fpga( void )
{
  unsigned int address = DF_FIRMWARE_OFFSET;
  uint8_t buffer[DF_PAGE_SIZE];
  fpga_reset(  );
  fpga_begin_conf(  );

  while( !fpga_confdone(  ) ) {
    dataflash_block_read( address, buffer, sizeof( buffer ) );

    if( fpga_send_ps( buffer, sizeof( buffer ) ) == -EBADBLOCK ) {
      return -1;
    };
    address += DF_PAGE_SIZE;

  };
  return 0;
};

/** This function parses input string for address and optional data.
  * Format of the input string: AAAA[[,| |\\t|:]DDDD]. All numbers
  * are in hexadecimal.
  * Input string must be 0-terminated and should not contain '\\n','\\r'.
  * 
  * \param input - input string for parsing. Note, that function changes it.
  * \param address - parsed address
  * \param data - parsed data word if present in input string. 
  *               (*data) = -1 if data is not present in input line.
  *
  * \return  0 is parsing successfull
  *	    -1 on error
  */
int parse_address_data( char *input, int *address, int *data )
{
  char *s;

  s = strtok( input, ": \t," );
  if( s != NULL ) {
    //( *address ) = strtol( s, NULL, 16 );
  }
  else {
    return -1;
  }

  s = strtok( NULL, ": \t," );
  if( s != NULL ) {
    //( *data ) = strtol( s, NULL, 16 );
    return 0;
  }
  else {
    ( *data ) = -1;
    return 0;
  }
};


int usb_get_data( char * buf, int len )
{
  int i = 0;

  buf[0] = '\0';
  while ( i < len  ) { 
    buf[ i ] = getchar( );
    putchar( buf[ i ] ); //echo response

    if( ( buf[ i ] == '\n' ) || ( buf[i] == '\r' ) ) {
      buf[ i ] = '\0';
      return i - 1;
    }

    i++;
  }

  //if string too large, we just clear rx buffer
  char dummy = 0;
  do {
    dummy = getchar( );
    putchar( dummy ); // echo response
  } while( ( dummy != '\n' ) && ( dummy != '\r' ) );

  buf[len - 1] = '\0';

  return -1;
}

/** Read address and optional data from usb and execute read/write callbacks
  *  After reading address and data call functions for checking if address/data
  *  is in valid ranges. If they are call read callback if only data was received.
  *  or call write callback if data and address were received.
  *
  * \param read :function performing read.
  * \param write: function performing write.
  * \param addr_check: function for checking if address is in proper range
  * \param data_check: function for checking if data is in proper range
  *
  * \return return -1 on error, 0 otherwise
  */
int command_rw_callback( read_callback read, write_callback write,
                         checkranges_callback addr_check,
                         checkranges_callback data_check )
{
  char buf[25];
  int ret = usb_get_data( buf, 25 );

  if( ret > 0 ) {
    int a = 0, d = 0;
    ret = parse_address_data( buf, &a, &d );
    if( ret == 0 ) {
      if( addr_check( a ) )
        return -1;
      if( d < 0 ) {
        //printf( "\n\r%04x\n\r", read( a ) );
      }
      else {
        if( data_check( d ) )
          return -1;
        write( a, d );
      }
    }

    return 0;
  }
  return -1;
}

void command( cmd_callback cmd )
{
  if( cmd(  ) ) {
    puts( "\n\rERROR\n\r" );
    b5_set( CARD_ERR_LED );   //Red user led
  }
  else {
    puts( "\n\rOK\n\r" );
    b5_clear( CARD_ERR_LED ); //Red user led
  };
};

void dump_nregs_offs( uint16_t num, uint16_t offs ) 
{
  int i, j;

  for( i = offs; i < offs + num; i += 8 ) {
    //printf( "%08x :", i );
    for( j = 0; j < 8; j++ ) {
      //printf( " %04x", fpgaregs_read( i + j ) );
    };
    puts( "\r" );
  };
} 

/*@}*/