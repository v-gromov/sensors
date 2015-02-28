/** \file conio_usb.c
  * \brief This file contains abstraction of I/O functions for stdio
  */
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stm32_usb_desc.h>
#include <usb_lib.h>
#include "conio_usb.h"

extern USB_OTG_PCD_DEV USB_OTG_PCD_dev;

/** \brief Flag for determining if the USB transfer from device to host is in progress
  */
static volatile signed char tx_in_progress = FALSE;

/** \brief Buffer to hold data received from usb
  */
static unsigned char rx_buffer[VIRTUAL_COM_PORT_DATA_SIZE];

/** \brief Amount of data in rx_buffer
  * \sa rx_buffer
  */
static unsigned short rx_buffer_count = 0;

/** \brief Currend pointer to data in rx_buffer
  * \sa rx_buffer
  */
static unsigned short rx_buffer_index = 0;

/** \brief Sends part of the data from device to host via USB
  * 
  * \note Sends no more than VIRTUAL_COM_PORT_DATA_SIZE at once
  * \param data -- data to send
  * \param length -- the length of data array
  * \return the amount of data actually transfered
  * \sa tx_in_progress
  * \sa VIRTUAL_COM_PORT_DATA_SIZE
  * \sa console_transmit
  * \todo Optimize, to increase transmit speed
  */
unsigned int console_transmit_partial(const unsigned char* data, unsigned int length)
{
  unsigned int l;
  if (!length) return 0;
  l = length <= VIRTUAL_COM_PORT_DATA_SIZE ? length : VIRTUAL_COM_PORT_DATA_SIZE;
  while (tx_in_progress);
  tx_in_progress = TRUE;
  USB_SIL_Write(EP1_IN, (uint8_t*) data, l);
  return l;
}

/** \defgroup ConsoleHal HAL functions to create console
  * \{
  */

/** \brief Send data from device to host via USB
  * \param data -- data to send
  * \param length -- the length of data array
  * \sa console_transmit_partial
  */
void console_transmit(const unsigned char* data, unsigned int length)
{
  while (length)
  {
    unsigned int l;
    l = console_transmit_partial(data, length);
    data += l;
    length -= l;
  }
}

/** \}
  */

/** \brief Enables receiving of data from EP3_OUT
  *
  * This works as a flow control measure (in direction from HOST to device)
  */
void ep3_start_receive(void){
  USB_OTG_EP *ep;
  ep = OTGD_FS_PCD_GetOutEP(EP3_OUT);
  ep->xfer_len = VIRTUAL_COM_PORT_DATA_SIZE;
  ep->xfer_count = 0;
  ep->is_in = 0;
  ep->num = EP3_OUT & 0x7F;
  if (USB_OTG_PCD_dev.ep0state == 0)
  {
    OTGD_FS_EPStartXfer( ep );
  }
};


/** \brief Callback, executed when device finished transmitting the data from device to host
  *
  * Basically drops the flag tx_in_progress, allowing more data to be sent
  * \sa console_transmit_partial()
  * \sa tx_in_progress
  * \note Executed from interrupt
  */
void EP1_IN_Callback(void)				
{
  tx_in_progress = FALSE;
}

// Receive

/** \brief Callback, executed when data is received from host to device
 * 
 *  Save data received from EP3 to rx buffer.
 *  Actually rx buffer is the pipe between the USB and stdio.
 * \todo Check for possible race condition.
 * \todo Optimize code.
 */
void EP3_OUT_Callback(void)
{
  USB_OTG_EP *ep;
  ep = OTGD_FS_PCD_GetOutEP(EP3_OUT);
  if ( ep->xfer_len ) {
	  memcpy(rx_buffer,ep->xfer_buff,ep->xfer_len);
  };
  rx_buffer_count=ep->xfer_len;
  rx_buffer_index=0;
}

/** \brief This function tries to receive data from rx_buffer, without blocking
 *  \param data -- pointer to the area where to store data
 *  \param length -- maximal length of data to receive
 *  \return length of actually received data
 *  \todo check for race conditions.
 *  \todo optimize
 * 
 *  There are 3 possible scenarios:
 *  -# receive buffer is empty. We should start xfer and return with zero length read. \sa ep3_start_receive()
 *  -# receive buffer contains more data than function requires. Just copy data.
 *  -# receive buffer contains less or equal count than length. Copy data and start xfer. \sa ep3_start_receive()
 */
unsigned int console_receive_non_blocking(unsigned char* data, unsigned int length)
{
  unsigned int l;
  if (rx_buffer_index >= rx_buffer_count)
  {
    //no data in rx_buffer - clean the counters and start receive
    rx_buffer_count = 0;
    rx_buffer_index = 0;
    ep3_start_receive();
    return 0;
  }
  //How much data we have in buffer
  l = rx_buffer_count - rx_buffer_index;
  if (l > length) {
	  //we have more than we need - take only what we need
	  l = length;
  }
  length = l;
  while (l--) {//Copy data from rx_buffer to the data area
	  *data++ = rx_buffer[rx_buffer_index++];
  }
  return length;
}

/** \defgroup ConsoleHal HAL functions to create console
  * \{
  */

/** \brief Receive data from the usb (Host -> device). 
  * \note Blocks, until data is received.
  * \param data -- pointer to the area where to store data
  * \param length -- length of data to receive
  * \return length of received data
  * \sa console_receive_non_blocking()
  */
int console_receive(unsigned char* data, unsigned int length)
{
  unsigned int l;
  if (!length) return 0;
  do l = console_receive_non_blocking(data, length);
  while (!l);
  return l;
}

/** \}
  */
