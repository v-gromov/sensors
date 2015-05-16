#include <stdio.h>
#include <libopenstm32/rcc.h>
#include <libopenstm32/nvic.h>
#include <libopenstm32/exti.h>
#include <libopenstm32/timer.h>
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
//volatile uint32_t sensors_old[4]={2,3,4,5};
volatile uint32_t time=0;

char bits[4]={14,15,8,9};

void delay(uint32_t ms){
	uint32_t newtime = time + ms;
	while(time !=newtime) continue;
}

void putstr(const char* string){
    if(string == NULL) return;
    while(*string != 0){
	putchar(*string);
	string++;
    };
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

void timer_handler(void){
     uint32_t flags3,flags4;
     flags3 = TIM3_SR;//Берём статус таймера 3 (два датчика)
     flags4 = TIM4_SR;//Берём статус таймера 4 (два датчика)
     TIM3_SR = 0;//Сброс статуса (чтобы срабатывали прерывания дальше)
     TIM4_SR = 0;
     if(flags3 & TIM_SR_CC3IF){//Сработал захват таймера
//	sensors_old[0]=sensors[0];
//        sensors[0]=TIM3_CCR3;//Считать захваченный момент
	sensors[0]++;
     }
     if(flags4 & TIM_SR_CC3IF){
//	sensors_old[2]=sensors[2];
//    	sensors[2]=TIM4_CCR3;
	sensors[2]++;
     }
     if(flags3 & TIM_SR_CC4IF){
//	sensors_old[1]=sensors[1];
//    	sensors[1]=TIM3_CCR4;
	sensors[1]++;
     }
     if(flags4 & TIM_SR_CC4IF){
//	sensors_old[3]=sensors[3];
//    	sensors[3]=TIM4_CCR4;
	sensors[3]++;
     }
}

void timer_init(void){
     TIM3_PSC = 36000;//Предделитель таймера - на входе - 36МГц, 36МГц/36000 = таймер тикает раз в 1 миллисекунду (слишком редко)
     TIM4_PSC = 36000;
     TIM3_CCMR2 = 0;//Сбросить настройки таймера
     TIM4_CCMR2 = 0;
     TIM3_CCMR2 |= TIM_CCMR2_CC4S_IN_TI1|TIM_CCMR2_CC3S_IN_TI1;//Захват с каналов T4/T3
     TIM4_CCMR2 |= TIM_CCMR2_CC4S_IN_TI1|TIM_CCMR2_CC3S_IN_TI1;
     TIM3_CCMR2 |= TIM_CCMR2_IC3F_CK_INT_N_8 | TIM_CCMR2_IC4F_CK_INT_N_8;//Фильтр на входе таймера на 8 тактов (какой частоты и чего?)
     TIM4_CCMR2 |= TIM_CCMR2_IC3F_CK_INT_N_8 | TIM_CCMR2_IC4F_CK_INT_N_8;
     TIM3_CCER |= TIM_CCER_CC3P | TIM_CCER_CC4P;//Выбор захвата по фронту или по спаду
     TIM4_CCER |= TIM_CCER_CC3P | TIM_CCER_CC4P;
     TIM3_CCMR2 |= TIM_CCMR2_IC3PSC_OFF | TIM_CCMR2_IC4PSC_OFF;//Выключить предделитель для захвата - если магнитов больше одного - этой настройкой можно компенсировать количество
     TIM4_CCMR2 |= TIM_CCMR2_IC3PSC_OFF | TIM_CCMR2_IC4PSC_OFF;
     TIM3_CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E;//Разрешить захват
     TIM4_CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E;
     TIM3_CR1 |= TIM_CR1_CEN;//Разрешить счёт таймеру
     TIM4_CR1 |= TIM_CR1_CEN;
     TIM3_DIER |= TIM_DIER_CC3IE|TIM_DIER_CC4IE;//Включить прерывания по захвату от таймера
     TIM4_DIER |= TIM_DIER_CC3IE|TIM_DIER_CC4IE;
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

  timer_init();
  nvic_register_interrupt(NVIC_TIM3_IRQ, timer_handler);//Зарегистрировать прерывание от таймера 3
  nvic_register_interrupt(NVIC_TIM4_IRQ, timer_handler);
  nvic_enable_irq(NVIC_TIM3_IRQ);//Разрешить прерывание от таймера 3 
  nvic_enable_irq(NVIC_TIM4_IRQ);

};

unsigned char byte = 0x0;

int main( void )
{
  int i;
  hardware_setup(  );

  putstr( "Sensors\r\n" );

  while( 1 ) {
    putstr("BEGIN\r\n");
    putstr("time: ");
    print_int(time);
    putstr("\r\ncounters: ");
    for(i=0;i<4;i++){
	print_int(sensors[i]);
	putstr(" ");
    };
    putstr("\r\n");
//    puts("old:\r\n");
//    for(i=0;i<4;i++){
//	print_int(sensors_old[i]);
//    };
//    puts("RPM:\r\n");
//    for(i=0;i<4;i++){
//	int rpm=0;
//	rpm = sensors[i]-sensors_old[i]+65536;//Считаем разницу во времени.
//	rpm = rpm % 65536;//Компенсируем отрицательные значения, так как таймер 16-бит = 65536
//        if(rpm!=0){
//	   rpm = 30000/rpm;//Считаем обороты в минуту, так как таймер тикает 1 раз в 1 миллисекунду = то по формуле 60 секунд/2*период
//	}
//	print_int(rpm);
//    };
//    puts("\r\n");
    putstr("\r\n\r\nEND\r\n\r\n");
    if(b5_get(ONBOARD_LED)) b5_clear(ONBOARD_LED); else b5_set(ONBOARD_LED);
    delay(1000);
  };

  return 0;
}
