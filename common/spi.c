#include <libopenstm32/rcc.h>
#include <common/io.h>
#include <common/spi.h>

#include "stm32f10x_spi.h"

/** \file spi.c
 *  \brief This file contains functions for accessing,configuring spi. And for spi transactions
 *  \addtogroup SPI
 *  \{
 * 
 * SPI transfer routines
 */

/*@{*/

/// SPI registers Masks
#define CR1_CLEAR_Mask       ((uint16_t)0x3040)

/** Configure SPI1 interface.
 *  SPI1 works through PA4-PA7 pins:
 *    - GPIO5 - SPI1_SCK
 *    - GPIO4 - SPI1_NSS
 *    - GPIO7 - SPI1_MOSI
 *    - GPIO6 - SPI1_MISO
 *
 *  SPI1 Settings:
 *    - master mode
 *    - 8 bits 
 *    - LSB first
 *    - software control of NSS pin
 *
 *  SPI1 interface is connected to FPGA on the B5-E1-4 board.
 */
void spi1_setup( void )
{
  // turn on the SPI1 bus
  rcc_peripheral_enable_clock( &RCC_APB2ENR,
                               SPI1EN | IOPAEN | AFIOEN );

  // SCK, MOSI
  b5_set_mode( CARD_SPI1_SCK, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );

  b5_set_mode( CARD_SPI1_MOSI, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );
  // NSS
  b5_set_mode( CARD_SPI1_NSS, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_PUSHPULL );

  //MISO
  b5_set_mode( CARD_SPI1_MISO, GPIO_MODE_INPUT,
               GPIO_CNF_INPUT_FLOAT );

  uint16_t r = 0;
  r = SPI1->CR1;
  r &= CR1_CLEAR_Mask;

  r |= SPI_BaudRatePrescaler_4 |
    SPI_CPOL_Low | SPI_CPHA_1Edge | SPI_DataSize_8b |
    SPI_FirstBit_LSB | SPI_Mode_Master | SPI_NSS_Soft;

  SPI1->CR1 = r;
  SPI1->CR1 |= SPI_Enable;
  
  spi1_slave_select(0);
};

/*
 *  SPI2 works through PB12-PB15 pins:
 *    - GPIO12 - SPI2_NSS	- CRC_ERROR (set to INPUT!!!)
 *    - GPIO13 - SPI2_SCK	- DCLK
 *    - GPIO14 - SPI2_MISO	- nSTATUS
 *    - GPIO15 - SPI2_MOSI	- DATA
 *
 *  SPI2 Settings:
 *    - master mode
 *    - 8 bits 
 *    - LSB first
 *    - software control of NSS pin: NSS unused, this pin is used as status input
 *
 *  SPI2 interface is connected to Passive serial interface of FPGA on the B5-E1-4 board.
 */
void spi2_setup( void )
{
  // turn on the SPI2 bus
  rcc_peripheral_enable_clock( &RCC_APB1ENR, SPI2EN );
  rcc_peripheral_enable_clock( &RCC_APB2ENR, IOPBEN );

  // setting up the alternate functions
  b5_set_mode( CARD_SPI2_SCK, GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );

  b5_set_mode( CARD_SPI2_MOSI, GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );

  uint16_t r = 0;
  r = SPI2->CR1;
  r &= CR1_CLEAR_Mask;

  r |= SPI_BaudRatePrescaler_4 |
    SPI_CPOL_Low | SPI_CPHA_1Edge | SPI_DataSize_8b |
    SPI_FirstBit_LSB | SPI_Mode_Master | SPI_NSS_Soft;

  SPI2->CR1 = r;
  SPI2->CR1 |= SPI_Enable;
};

/** Configure SPI3 interface.
 *  SPI3 works through PC10-PC12 pins:
 *    - GPIO10 - SPI3_SCK
 *    - GPIO11 - SPI3_MISO
 *    - GPIO12 - SPI3_MOSI
 *
 *  NSS is controlled softwarely via simple GPIO port:
 *    - PD2 - SPI3_NSS
 *
 *  SPI3 Settings:
 *    - master mode
 *    - 8 bits 
 *    - MSB first
 *    - software control of NSS pin
 *    - maximum speed
 *
 *  SPI3 interface is connected to DataFlash on the B5-MOD-MC board.
 */
void spi3_setup( void )
{
  rcc_peripheral_enable_clock( &RCC_APB2ENR,
                               IOPCEN | IOPDEN | AFIOEN );
  rcc_peripheral_enable_clock( &RCC_APB1ENR, SPI3EN );

  // remappind SPI3 to port C
  AFIO_MAPR |= AFIO_MAPR_SPI3_REMAP;

  // SCK, MOSI
  b5_set_mode( CARD_SPI3_SCK, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );

  b5_set_mode( CARD_SPI3_MOSI, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL );
  // NSS
  b5_set_mode( CARD_SPI3_NSS, GPIO_MODE_OUTPUT_50_MHZ,
               GPIO_CNF_OUTPUT_PUSHPULL );

  //MISO
  b5_set_mode( CARD_SPI3_MISO, GPIO_MODE_INPUT,
               GPIO_CNF_INPUT_FLOAT );

  unsigned short r = SPI3->CR1;
  r &= CR1_CLEAR_Mask;
  r |= SPI_BaudRatePrescaler_4 |
    SPI_CPOL_Low |
    SPI_CPHA_1Edge |
    SPI_DataSize_8b |
    SPI_FirstBit_MSB | SPI_Mode_Master | SPI_NSS_Soft;
  SPI3->CR1 = r;
  SPI3->CR1 |= SPI_Enable;

  spi3_slave_select(0);
};

/** \addtogroup GPIO
  * \{
  */

/** SPI3 NSS control. Implemented via simple GPIO pin.
 *
 * \param state -- 1 means active(gnd). 0 means inactive NSS state (vcc).
 */
void spi3_slave_select( unsigned char state )
{
  if( state ) {
    b5_clear( CARD_SPI3_NSS );
  }
  else {
    b5_set( CARD_SPI3_NSS );
  };
};

/** \}
  */

/** Simplest spi transferring function.
 *  It receives/transmits 'n' bytes.
 *
 *  \param n -- length of 'rx' and 'tx' vectors
 *  \param tx -- data for transmit
 *  \param rx -- buffer for received data
 *  \param spi -- spi bus to perform transfer on
 *
 */
void spi_transfer( SPI_TypeDef * spi, unsigned short n,
                   unsigned char *tx, unsigned char *rx )
{
  while( n-- ) {
    unsigned char b = tx ? ( *tx++ ) : 0xff;

    spi->DR = b;

    while( ( spi->SR & SPI_I2S_FLAG_RXNE ) == 0 ) {
      // wait while receive buffer empty.
    }

    b = spi->DR;

    if( rx ) {
      *rx++ = b;
    }
  }
}

/** SPI3 transfer function. Differs from spi_transfer 
 *  because softwarely controls NSS.
 *
 *  \sa spi_transfer
 */
void spi3_transfer( unsigned short n,
                    unsigned char *tx, unsigned char *rx )
{
  spi3_slave_select( 1 );
  spi_transfer( SPI3, n, tx, rx );
  spi3_slave_select( 0 );
}

/** \addtogroup GPIO
  * \{
  */

/** SPI1 NSS control. Implemented via simple GPIO pin.
 *
 * \param state -- 1 means active(gnd). 0 means inactive NSS state (vcc).
 */
void spi1_slave_select( unsigned char state )
{
  if( state ) {
    b5_clear( CARD_SPI1_NSS );
  }
  else {
    b5_set( CARD_SPI1_NSS );
  };
}

/** \}
  */

/** SPI1 transfer function. Differs from spi_transfer 
 *  because softwarely controls NSS.
 *
 *  \sa spi_transfer
 *  \sa spi3_transfer
 */
void spi1_transfer( unsigned short n,
                    unsigned char *tx, unsigned char *rx )
{
  spi1_slave_select( 1 );
  spi_transfer( SPI1, n, tx, rx );
  spi1_slave_select( 0 );
}

/*@}*/
