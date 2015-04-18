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

}


void common_gpio_setup( void )
{
  rcc_peripheral_enable_clock( &RCC_APB2ENR,
                               IOPEEN | IOPDEN | IOPBEN | IOPCEN | AFIOEN );
  rcc_peripheral_enable_clock( &RCC_APB1ENR, TIM3EN|TIM4EN);

  //Sensors
  b5_set_mode( SENSOR1, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT);
  b5_set_mode( SENSOR2, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT);
  b5_set_mode( SENSOR3, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT);
  b5_set_mode( SENSOR4, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT);
  //LED
  b5_set_mode( ONBOARD_LED, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL);
  AFIO_MAPR |= AFIO_MAPR_TIM4_REMAP | AFIO_MAPR_TIM3_REMAP_FULL_REMAP;
};

/** \}
  */
