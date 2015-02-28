#include <common/power.h>
#include <libstm32usb/udelay.h>
#include <common/io.h>

void power_init( void )
{
  //nPWREN
  //Bugfix that prevents the conroller flushing 
  b5_set_mode(CARD_POWER,GPIO_MODE_INPUT,GPIO_CNF_INPUT_PULL_UPDOWN);
  udelay(3);
  b5_set_mode(CARD_POWER,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL);
};

void power_on( void )
{
  b5_clear(CARD_POWER);
  udelay(300);
  b5_set(CARD_PWR_LED);
};

void power_off( void )
{
  b5_set(CARD_POWER);
  b5_clear(CARD_PWR_LED);
};

// \return 0 - when power off
// \return 1 - when power is on
int power_get( void )
{
  return b5_get(CARD_POWER)?0:1;
};
