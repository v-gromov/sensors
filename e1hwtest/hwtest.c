#include <stdio.h>
#include <libopenstm32/rcc.h>
#include <libopenstm32/nvic.h>
#include <stm32_usb_init.h>
#include <stm32_usb_pwr.h>
#include <libusbserial/stm32_otg_hw_config.h>
#include <libstm32usb/stm32_otgd_fs_dev.h>


#include <common/spi.h>
#include <common/usart2.h>
#include <common/i2c.h>
#include <common/fpga.h>
#include <common/fpgaregs.h>
#include <common/service.h>
#include <common/power.h>
#include <udelay.h>


#include "gpio.h"
#include "fpgaregs.h"
#include "cmdlist.h"
#include "dumpregs.h"
#include "autotest.h"
#include "conio.h"
#include "conio_usb.h"

#ifdef _LINK_RAM_
  #include <nvic.h>
#endif

void __aeabi_idiv(void){
}

/** NOTE: this function is called before FPGA is loaded.
 */
void hardware_setup( void )
{
  //For running at proper speed
  rcc_clock_setup_in_hse_25mhz_out_72mhz(  );
  gpio_setup(  );
  
#ifdef _LINK_RAM_
  nvic_init( );
#endif

  //init USB  
  Set_USBClock(  );
  nvic_enable_irq( NVIC_OTG_FS_IRQ );
  USB_Init(  );
  USB_DevDisconnect();
  udelay( 3000 );
  USB_DevConnect();
  while( bDeviceState != CONFIGURED )
    continue;

  conio_init(console_transmit,console_receive);

  //I2C init
  TwInit( 0 );

};

unsigned char byte = 0x0;

int main( void )
{
  int i;
  hardware_setup(  );

  puts( "B5-E1\r\n" );

  while( 1 ) {
    byte = getchar( );
//    putchar( byte );

    switch ( byte ) {
      case 'p':                // Program FPGA from Dataflash
        command( df2fpga );
        break;
      case 'f':                // Access FPGA registers (write, read) format: f<address><separator>[<data>]
        command( fpgaregs_rw );
        break;
      case 'D':		       // Dump all registers (FPGA, Liu, Codec)
        dump_registers( );
      case 'v':                // Turn on/off power to the board. Format v[0|1].
        //                        command( power );
        break;
      case 'h':
        puts( cmdlist );
        break;
      default:
        puts( "ERROR invalid command\r\n" );
        puts( cmdlist );
        break;
    }
  };

  return 0;
}
