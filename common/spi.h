#ifndef SPI__H
#define SPI__H

/** \addtogroup SPI
  * \{
  */

/*@{*/
#include <libopenstm32/spi.h>
#include "stm32f10x_spi.h"

void spi_transfer( SPI_TypeDef* spi,
                   unsigned short n,
                   unsigned char *tx,
                   unsigned char *rx );

/*
 * SPI1
 */
void spi1_setup();
void spi1_slave_select( unsigned char );
void spi1_transfer( unsigned short n, 
                   unsigned char *tx, unsigned char *rx );

/*
 * SPI2
 */
void spi2_setup();

/*
 * SPI3
 */
void spi3_setup();
void spi3_transfer( unsigned short n, 
                   unsigned char* tx, unsigned char* rx );
void spi3_slave_select( unsigned char state );
                   

/*@}*/
#endif // SPI__H
