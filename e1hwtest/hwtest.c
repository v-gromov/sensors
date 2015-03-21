#include <stdio.h>
#include <libopenstm32/rcc.h>
#include <libopenstm32/nvic.h>
#include <libopenstm32/exti.h>
#include <stm32_usb_init.h>
#include <stm32_usb_pwr.h>
#include <libusbserial/stm32_otg_hw_config.h>
#include <libstm32usb/stm32_otgd_fs_dev.h>

#include <common/i2c.h>
#include <common/service.h>
#include <udelay.h>


#include "gpio.h"
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
  
/*
  exti_set_trigger(EXTI8, EXTI_TRIGGER_RISING);
  exti_set_trigger(EXTI9, EXTI_TRIGGER_RISING);
  exti_set_trigger(EXTI14, EXTI_TRIGGER_RISING);
  exti_set_trigger(EXTI15, EXTI_TRIGGER_RISING);
  exti_select_source(EXTI8, GPIOC);
  exti_select_source(EXTI9, GPIOC);
  exti_select_source(EXTI14, GPIOD);
  exti_select_source(EXTI15, GPIOD);
  exit_enable_request(EXTI8|EXTI9|EXTI14|EXTI15);
*/
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
      case 'h':
        puts( "This is help\r\n" );
        break;
      default:
        puts( "ERROR invalid command\r\n" );
        break;
    }
  };

  return 0;
}
