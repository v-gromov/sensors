#include <libopenstm32/rcc.h>
#include <common/io.h>
#include <common/power.h>

void common_gpio_setup( void )
{
  rcc_peripheral_enable_clock( &RCC_APB2ENR,
                               IOPEEN | IOPDEN | IOPBEN );


  //Power & Error LEDs
  b5_set_mode( CARD_PWR_LED, GPIO_MODE_OUTPUT_2_MHZ,
               GPIO_CNF_OUTPUT_PUSHPULL );
  b5_clear( CARD_PWR_LED );

  b5_set_mode( CARD_ERR_LED, GPIO_MODE_OUTPUT_2_MHZ,
               GPIO_CNF_OUTPUT_PUSHPULL );
  b5_clear( CARD_ERR_LED );

  //USART2 TX
  b5_set_mode( CARD_USART2_TX, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );
  //USART2 RX
  b5_set_mode( CARD_USART2_RX, GPIO_MODE_INPUT,
               GPIO_CNF_INPUT_FLOAT );
  
  power_init(  );

  //nCONFIG
  b5_set_mode( CARD_NCONFIG, GPIO_MODE_OUTPUT_2_MHZ,
               GPIO_CNF_OUTPUT_OPENDRAIN );

  //CONF_DONE
  b5_set_mode( CARD_CONF_DONE, GPIO_MODE_INPUT,
               GPIO_CNF_INPUT_FLOAT );

  //nSTATUS
  b5_set_mode( CARD_NSTATUS, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT );

  //CRC_ERR
  b5_set_mode( CARD_CRC_ERR, GPIO_MODE_OUTPUT_2_MHZ,
               GPIO_CNF_INPUT_FLOAT );

};
