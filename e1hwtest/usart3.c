#include <libopenstm32/rcc.h>
#include <libopenstm32/gpio.h>
#include <libopenstm32/usart.h>

/** \file usart3.c
  * \brief This file contains setup function for USART3
  * \addtogroup USART
  * \{
  */

void usart3_setup( void )
{
  /* Enable clocks for GPIO port D (for GPIO_USART3_TX) and USART3. */
  rcc_peripheral_enable_clock( &RCC_APB2ENR, IOPDEN );
  rcc_peripheral_enable_clock( &RCC_APB2ENR, AFIOEN );
  rcc_peripheral_enable_clock( &RCC_APB1ENR, USART3EN );

  AFIO_MAPR |= AFIO_MAPR_USART3_REMAP_FULL_REMAP;
  usart_set_baudrate( USART3, 4500000 );
  usart_set_databits( USART3, 8 );
  usart_set_stopbits( USART3, USART_STOPBITS_1 );
  usart_set_mode( USART3, USART_MODE_TX | USART_MODE_RX );
  usart_set_parity( USART3, USART_PARITY_NONE );
  usart_set_flow_control( USART3, USART_FLOWCONTROL_NONE );
  USART_CR2( USART3 ) |= USART_CR2_CLKEN|USART_CR2_LBCL;
  usart_enable( USART3 );
};

/** \}
  */
