/** \file dumpregs.c
  * \brief Function for dumping register contents
  */
#include <stdio.h>
#include <common/service.h>

/** \brief Dump FPGA, LIU and CODEC registers on console
  *
  * \sa fpgaregs_read()
  * \sa liuregs_read()
  * \sa codec_read()
  */
void dump_registers( void )
{
  puts( "\n\rFPGA\r" );
  puts( "\n\rLIU\r" );
  puts( "\n\rCODEC\r" );
  puts( "\r" );
}

