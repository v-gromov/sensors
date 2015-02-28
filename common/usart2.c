#include <libopenstm32/rcc.h>
#include <libopenstm32/gpio.h>
#include <libopenstm32/usart.h>

/** \file usart2.c
  * \brief This file contains functions for init and access to the USART2
  * \addtogroup USART
  * \{
  */

/*@{*/
  
void usart2_setup( void )
{
  /* Enable clocks for GPIO port D (for GPIO_USART2_TX) and USART2. */
  rcc_peripheral_enable_clock( &RCC_APB2ENR, IOPDEN );
  rcc_peripheral_enable_clock( &RCC_APB2ENR, AFIOEN );
  rcc_peripheral_enable_clock( &RCC_APB1ENR, USART2EN );
  AFIO_MAPR |= AFIO_MAPR_USART2_REMAP;
  usart_set_baudrate( USART2, 115200 );
  usart_set_databits( USART2, 8 );
  usart_set_stopbits( USART2, USART_STOPBITS_1 );
  usart_set_mode( USART2, USART_MODE_TX | USART_MODE_RX );
  usart_set_parity( USART2, USART_PARITY_NONE );
  usart_set_flow_control( USART2, USART_FLOWCONTROL_NONE );
  usart_enable( USART2 );
};

/** Function reads usart2 data until '\n' received.
 *  If received string is longer than buffer size then 
 *  it returns -1 and discards remaining string.
 *  
 * \param buf -- buffer for receiving line
 * \param len -- length of the buffer
 * \return received string length or -1 if string is too large
 */
int usart2_rcv_line( char *buf, int len )
{
  int i = 0;

  buf[0] = '\0';

  while( i < len ) {
    buf[i] = usart_recv( USART2 );
    usart_send( USART2, buf[i] );       // echo

    if( ( buf[i] == '\n' ) || ( buf[i] == '\r' ) ) {
      buf[i] = '\0';
      return i - 1;
    }

    i++;
  }

  // string is too large. skipping remain part.
  char dummy = 0;
  do {
    dummy = usart_recv( USART2 );
    usart_send( USART2, dummy );        // echo
  } while( ( dummy != '\n' ) && ( dummy != '\r' ) );

  buf[len - 1] = '\0';

  return -1;
}

/*@}*/
