#include <stdio.h>
#include <libopenstm32/rcc.h>
#include <libopenstm32/nvic.h>
#include <libopenstm32/exti.h>
#include <libopenstm32/systick.h>
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

volatile uint32_t sensors[4]={0,0,0,0};
volatile uint32_t sensors_old[4]={0,0,0,0};
volatile uint32_t time=0;

char bits[4]={14,15,8,9};

void print_int(uint32_t integer_n){
    char buffer[11];
    int i;
    for(i=0;i<sizeof(buffer);i++){
	buffer[i]='0';
    };
    buffer[10]=0;
    i=9;
    while(integer_n>0){
	int modulus;
	modulus=integer_n % 10;
	buffer[i]=char('0'+modulus);
	i--;
	integer_n /=10;
    }
    puts(buffer);
}

void systick_handler( void){
    time ++;
}

void exti_handler( void ){
    int i;
    for(i=0;i<4;i++){
	    if(EXTI_PR & (1<<bits[i])){
		sensors_old[i]=sensors[i];
		sensors[i]=time;
		EXTI_PR |= (1<<bits[i]);
	    }
    }
}

void exti_init( void) {
    EXTI_IMR = 0xc300;
    EXTI_EMR = 0xc300;
    EXTI_RTSR = 0xc300;
    EXTI_FTSR = 0xc300;
    AFIO_EXTICR3 = 0x0022;
    AFIO_EXTICR4 = 0x3300;
}

/** NOTE: this function is called before FPGA is loaded.
 */
void hardware_setup( void )
{
  //For running at proper speed
  rcc_clock_setup_in_hse_25mhz_out_72mhz(  );
  gpio_setup(  );
  exti_init( );
  
#ifdef _LINK_RAM_
  nvic_init( );
#endif
  nvic_register_interrupt(NVIC_SYSTICK_IRQ,systick_handler);
  systick_set_reload(9000);
  systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB_DIV8);
  systick_counter_enable();
  systick_interrupt_enable();
  nvic_enable_irq(NVIC_SYSTICK_IRQ);

  nvic_register_interrupt(NVIC_EXTI0_IRQ,exti_handler);
  nvic_register_interrupt(NVIC_EXTI1_IRQ,exti_handler);
  nvic_register_interrupt(NVIC_EXTI2_IRQ,exti_handler);
  nvic_register_interrupt(NVIC_EXTI3_IRQ,exti_handler);
  nvic_register_interrupt(NVIC_EXTI4_IRQ,exti_handler);
  nvic_enable_irq(NVIC_EXTI0_IRQ);
  nvic_enable_irq(NVIC_EXTI1_IRQ);
  nvic_enable_irq(NVIC_EXTI2_IRQ);
  nvic_enable_irq(NVIC_EXTI3_IRQ);
  nvic_enable_irq(NVIC_EXTI4_IRQ);


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
    for(i=0;i<3;i++){
	print_int(sensors[0]);
	puts(" ");
    };
    puts("\n");
    for(i=0;i<3;i++){
	print_int(sensors_old[0]);
	puts(" ");
    };
    puts("\n\n");
  };

  return 0;
}
