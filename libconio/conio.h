/** \brief This file contains typedefs for conio callbacks
  * \author Nikolaj Zamotaev
  * \file conio.h
  */
#ifndef _CONIO_H_
#define _CONIO_H_

/** \defgroup callbacks
  * \{
  */
/** \brief Function type for writing data to console
  * \sa conio_init()
  * \sa _write()
  */
typedef void (*transmit_callback)(const unsigned char* data,unsigned int length);
/** \brief Function type for reading data from console
  * \sa conio_init()
  * \sa _read()
  */
typedef int (*receive_callback)(unsigned char* data,unsigned int length);

/** \}
  */

void conio_init(transmit_callback tx_function,receive_callback rx_function);


#endif
