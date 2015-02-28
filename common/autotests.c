#include <common/fpga.h>
#include "fpgaregs.h"

/** \brief FPGA registers access autotest
  * \param address - fpga address to check for accesibility
  * \return 0 if test is ok, 1 otherwise
  */
int fpga_reg_autotest(int address ) 
{
  int flag = 0;
  fpgaregs_write( address, 0x0000 );
  if( fpgaregs_read( address ) != 0x0000 )
    flag = 1;
  fpgaregs_write( address, 0xa55a );
  if( fpgaregs_read( address ) != 0xa55a )
    flag = 1;
  fpgaregs_write( address, 0x5aa5 );
  if( fpgaregs_read( address ) != 0x5aa5 )
    flag = 1;
  fpgaregs_write( address, 0xffff );
  if( fpgaregs_read( address ) != 0xffff )
    flag = 1;
  return flag;
};
