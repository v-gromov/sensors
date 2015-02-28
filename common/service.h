#ifndef SERVICE__H
#define SERVICE__H

#include <stdint.h>

/** \addtogroup SERVICE 
  * \{
  */

/*@{*/
#define DF_FIRMWARE_OFFSET  0x00

typedef int ( *read_callback ) ( int );
typedef int ( *write_callback ) ( int, int );
typedef int ( *checkranges_callback ) ( int );
typedef int ( *cmd_callback ) ( void );

int df2fpga( void );
int parse_address_data( char *input, int *address, int *data );
int command_rw_callback( read_callback read, write_callback write,
                         checkranges_callback check_addr,
                         checkranges_callback check_data );

void command( cmd_callback cmd );
void dump_nregs_offs( uint16_t num, uint16_t offs ); 
/*@}*/
#endif // SERVICE__H
