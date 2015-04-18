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

volatile uint32_t sensors[4]={1,2,3,4};
volatile uint32_t sensors_old[4]={2,3,4,5};
volatile uint32_t time=0;

char bits[4]={14,15,8,9};

void delay(uint32_t ms){
	uint32_t newtime = time + ms;
	while(time !=newtime) continue;
}

void print_int(uint32_t integer_n){
    char buffer[12]=" 0000000000";
    int i;
    i=10;
    while(integer_n>0){
	int modulus;
	modulus=integer_n % 10;
	buffer[i]=(char)('0'+modulus);
	i--;
	integer_n /=10;
    }
    for(i=0;buffer[i]!=0;i++) putchar(buffer[i]);
}

void systick_handler( void){
    time ++;
}

void exti_handler( void ){
    int i;
    uint32_t flags = EXTI_PR;
    EXTI_PR = 0xffff;
    if(b5_get(ONBOARD_LED)) b5_clear(ONBOARD_LED); else b5_set(ONBOARD_LED);
    for(i=0;i<4;i++){
	    if(flags & (1<<bits[i])){
		sensors_old[i]=sensors[i];
		sensors[i]=time;
	    }
    }
}

void exti_init( void) {
    EXTI_IMR = 0xc300;
    EXTI_EMR = 0xc300;
    EXTI_RTSR = 0xc300;
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
  
//#ifdef _LINK_RAM_
  nvic_init( );
//#endif

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
  exti_init( );
  EXTI_PR = 0xffff;
  nvic_enable_irq(NVIC_EXTI0_IRQ);
  nvic_enable_irq(NVIC_EXTI1_IRQ);
  nvic_enable_irq(NVIC_EXTI2_IRQ);
  nvic_enable_irq(NVIC_EXTI3_IRQ);
  nvic_enable_irq(NVIC_EXTI4_IRQ);

};

unsigned char byte = 0x0;

int main( void )
{
  int i;
  hardware_setup(  );

  puts( "B5-E1\r\n" );

  while( 1 ) {
    puts("time:\r\n");
    print_int(time);
    puts("new:\r\n");
    for(i=0;i<4;i++){
	print_int(sensors[i]);
    };
    puts("");
    puts("old:\r\n");
    for(i=0;i<4;i++){
	print_int(sensors_old[i]);
    };
    puts("\r\n");
    if(b5_get(ONBOARD_LED)) b5_clear(ONBOARD_LED); else b5_set(ONBOARD_LED);
    delay(1000);
  };

  return 0;
}
