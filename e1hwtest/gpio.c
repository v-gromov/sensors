#include <libopenstm32/rcc.h>
#include "gpio.h"

/** \file gpio.c
  * \brief This file contains gpio initialisation for E1 card
  * \addtogroup GPIO
  * \{
  */

void gpio_setup( void )
{
  common_gpio_setup();

  // USART3 Clock
  b5_set_mode( CARD_US3_CLK, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);

  // USART3 RX
  b5_set_mode( CARD_US3_RX, GPIO_MODE_INPUT,
               GPIO_CNF_INPUT_FLOAT );

  // USART3 TX
  b5_set_mode( CARD_US3_TX, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );

  // IDT82v2084 liu chip select
  b5_set_mode( CARD_LIU_CS, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_PUSHPULL );

  // IDT82v2084 Reset
  b5_set_mode( CARD_LIU_RST, GPIO_MODE_OUTPUT_2_MHZ,
               GPIO_CNF_OUTPUT_PUSHPULL );

  // Codec reset
  b5_set_mode( CARD_CODEC_RST, GPIO_MODE_OUTPUT_2_MHZ,
               GPIO_CNF_OUTPUT_PUSHPULL );

}


void common_gpio_setup( void )
{
  rcc_peripheral_enable_clock( &RCC_APB2ENR,
                               IOPEEN | IOPDEN | IOPBEN | IOPCEN );


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

/** \}
  */
