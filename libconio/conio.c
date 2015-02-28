/** \file conio.c
  * \brief This file contains I/O functions for stdio
  */
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "conio.h"


/** \defgroup callbacks Console management functions
  * \{
  */

/** \brief Function used for writing data to console
  * \sa conio_init()
  * \sa _write()
  */
transmit_callback cons_transmit=NULL;
/** \brief Function for receiving data from console
  * \sa conio_init()
  * \sa _read()
  */
receive_callback  cons_receive=NULL;

/** \brief Sets the callbacks for the conio library.
  * \param tx_function -- function for transmitting data from stm32
  * \param rx_function -- function for receiving data into stm32
  * \note This function must be called before using printf/scanf
  */
void conio_init(transmit_callback tx_function,receive_callback rx_function){
	cons_transmit=tx_function;
	cons_receive=rx_function;
};

/** \}
  */

int _isatty( int file )
{
  file = 0;

  return 1;
}

int _read( int file, char *ptr, int len )
{
  file = 0;

  if( len < 0 )
    return -1;
  if(cons_receive==NULL) return -1;
  return cons_receive((unsigned char*) ptr, len);
}

int _write( int file, char *ptr, int len )
{
  file = 0;

  if( len < 0 )
    return -1;

  if(cons_transmit==NULL) return -1;
  cons_transmit((unsigned char*) ptr, len);
  return len;
}

int _fstat( int fd, struct stat *buf )
{
  fd = ( int ) buf;
  return 0;
}

off_t _lseek( int fd, off_t offset, int whence )
{
  fd = whence;
  fd = offset;
  return 0;
}

int _close( int fd )
{
  fd = 0;
  return 0;
}

int _open(const char* path,int flags)
{
  path=NULL;
  flags=0;
  return 0;
};

/*
 sbrk
 Increase program data space.
 Malloc and related functions depend on this
 */
caddr_t _sbrk(int incr) {

    extern char end;
    extern char _estack;
    static char *heap_end = 0;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &end;
    }
    prev_heap_end = heap_end;

//     char * stack = (char*) __get_MSP();
     if ((heap_end+ incr) > (_estack-1024*4))
     {
         errno = ENOMEM;
         return  (caddr_t) -1;
         //abort ();
     }

    heap_end += incr;
    return (caddr_t) prev_heap_end;

}

