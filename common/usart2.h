#ifndef USART2__H
#define USART2__H

/** \addtogroup USART
  * \{
  */

/*@{*/
void usart2_setup();
int usart2_rcv_line( char* buf, int len );

/*@}*/
#endif // USART2__H
