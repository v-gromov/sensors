/** \file conio_usb.h
  * \brief This file contains abstraction of I/O functions for stdio
  */
#ifndef _CONIO_USB_H_
#define _CONIO_USB_H_

void console_transmit(const unsigned char* data, unsigned int length);
int console_receive(unsigned char* data, unsigned int length);

#endif
