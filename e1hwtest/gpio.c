#include <common/io.h>

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

/** \}
  */
