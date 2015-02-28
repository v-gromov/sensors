/** \file pca9532.c
  * \brief Functions to access pca9532 chip on i2c
  */

/*!
 * \addtogroup LibCommon
 */

/*@{*/

#include "pca9532.h"

#include <common/i2c.h>
#include <common/service.h>
#include <common/usart2.h>

#define GPIO_CFG_OUTPUT  0
#define GPIO_CFG_PULLUP  1

/** \brief Get pin value
  *
  * \param bit -- pin number to read
  * \param value -- place to store pin value
  */
int Pca9532GetBit( int bit, int *value )
{
  unsigned char val;
  if( bit <= 7 ) {
    TwMasterRegRead( PCA9532_ADDR, PCA_INPUT0, 1, &val, 1 );
  }
  else {
    TwMasterRegRead( PCA9532_ADDR, PCA_INPUT1, 1, &val, 1 );
  };
  *value = ( val & ( 1 << ( bit % 8 ) ) );
  return 0;
}

/** \brief switch pin to low logic level (light the led)
  *
  * \param bit -- pin number to switch to low
  */
int Pca9532SetBitLow( int bit )
{
  unsigned char temp_val;
  unsigned char reg_addr;
  reg_addr = bit / 4;
  TwMasterRegRead( PCA9532_ADDR, PCA_LS0 + reg_addr, 1, &temp_val, 1 );
  PCA_SET_STATE( temp_val, bit, STATE_LOW );
  TwMasterRegWrite( PCA9532_ADDR, PCA_LS0 + reg_addr, 1, &temp_val, 1 );
  return 0;
}

/** \brief switch pin to high logic level
  *
  * \param bit -- pin number to switch to high
  */
int Pca9532SetBitHigh( int bit )
{
  unsigned char temp_val;
  unsigned char reg_addr;
  reg_addr = bit / 4;
  TwMasterRegRead( PCA9532_ADDR, PCA_LS0 + reg_addr, 1, &temp_val, 1 );
  PCA_SET_STATE( temp_val, bit, STATE_HIGH );
  TwMasterRegWrite( PCA9532_ADDR, PCA_LS0 + reg_addr, 1, &temp_val, 1 );
  return 0;
}


/** \brief Configure pin
  *
  * \param bit -- pin number to configure
  * \param flags -- configuration flags of pin
  *
  * \todo do we need this function?
  *
  */
static int Pca9532PinConfigSet( int bit, uint32_t flags )
{
  if( flags & GPIO_CFG_PULLUP )
    Pca9532SetBitHigh( bit );
  return 0;
}


/** \brief Configure PWM0 generator
  *
  * \param period -- PWM period in milliseconds
  * \param duty -- duty cycle (from 0 to 255), 127 = 50%
  */
static int Pca9532Pwm0Config( int period, int duty )
{
  unsigned char data[2];
  data[0] = ( period * 44 ) / 1000 - 1;
  data[1] = duty;
  TwMasterRegWrite( PCA9532_ADDR, PCA_PSC0 + PCA_AUTOINC, 1, data, 2 );
  return 0;
};


/** \brief Configure PWM1 generator
  *
  * \param period -- PWM period in milliseconds
  * \param duty -- duty cycle (from 0 to 255), 127 = 50%
  */
static int Pca9532Pwm1Config( int period, int duty )
{
  unsigned char data[2];
  data[0] = ( period * 44 ) / 1000 - 1;
  data[1] = duty;
  TwMasterRegWrite( PCA9532_ADDR, PCA_PSC1 + PCA_AUTOINC, 1, data, 2 );
  return 0;
};


/** \brief initialise pca9532 chip
  *
  * Sets all outputs to logic HIGH (pullup) state.
  * Sets PWM controllers to output signal with 50% duty cycle and 1 second period.
  */
int Pca9532Init( void )
{
  int i;
  Pca9532Pwm0Config( 1000, 127 );       //1 секунда период, 50% цикл
  Pca9532Pwm1Config( 1000, 127 );       //аналогично
  for( i = 0; i < 15; i++ ) {
    Pca9532PinConfigSet( i, GPIO_CFG_OUTPUT );
    Pca9532SetBitHigh( i );
  };
  return 0;
}


/** Check data range 
  * \sa command_rw_callback
  */
static int data_check( int data )
{
  if( data < 0 )
    return -1;
  if( data > 256 )
    return -1;
  return 0;
}


/** Check number range for led
  * \sa command_rw_callback
  */
static int address_led_check( int address )
{
  if( address < 0 )
    return -1;
  if( address > MAX_LED )
    return -1;
  return 0;
};


/** Check number range for resistors
  * \sa command_rw_callback
  */
static int address_res_check( int address )
{
  if( address < 0 )
    return -1;
  if( address > MAX_RES )
    return -1;
  return 0;
}


/** \brief Read led state
  *
  * \param address -- number of led to get state
  * \return 1 if led is lit, 0 otherwise
  * \sa command_rw_callback
  */
static int leds_read( int address )
{
  int value;
  Pca9532GetBit( address, &value );
  return value ? 0 : 1;
}

/** \brief Set led state
  *
  * \param address -- number of led to set
  * \param data -- new state
  * \return the value of data parameter
  * \sa command_rw_callback
  */
static int leds_write( int address, int data )
{
  if( data ) {
    Pca9532SetBitLow( address );
  }
  else {
    Pca9532SetBitHigh( address );
  };
  return data;
}

/** \brief Read "short" resistors state
  *
  * \sa command_rw_callback
  * \param address -- number of port for which to read resistor state
  * \return 0 when resistor is disabled, 1 otherwise
  */
static int short_read( int address )
{
  int value;
  Pca9532GetBit( 2 * address + 9, &value );
  return value ? 0 : 1;
}

/** \brief Set "short" resistors state
  *
  * \sa command_rw_callback
  * \param address -- number of port for which to set resistor state
  * \param data -- new state
  * \return the value of data parameter
  */
int short_write( int address, int data )
{
  if( data ) {
    Pca9532SetBitHigh( 2 * address + 9 );
  }
  else {
    Pca9532SetBitLow( 2 * address + 9 );
  };
  return data;
}

/** \brief Read loading resistors state
  *
  * \sa command_rw_callback
  * \param address -- number of port for which to read resistor state
  * \return 0 when resistor is disabled, 1 otherwise
  */
static int load_read( int address )
{
  int value;
  Pca9532GetBit( 2 * address + 8, &value );
  return value ? 0 : 1;
}

/** \brief Set loading resistors state
  *
  * \sa command_rw_callback
  * \param address -- number of port for which to set resistor state
  * \param data -- new state
  * \return the value of data parameter
  */
int load_write( int address, int data )
{
  if( data ) {
    Pca9532SetBitLow( 2 * address + 8 );
  }
  else {
    Pca9532SetBitHigh( 2 * address + 8 );
  };
  return data;
}

/** \sa command_rw_callback
  */
int leds_set( void )
{
  return command_rw_callback( leds_read, leds_write,
                              address_led_check, data_check );
}

/** \sa command_rw_callback
  */
int resistors_load_set( void )
{
  return command_rw_callback( load_read, load_write,
                              address_res_check, data_check );
};

/** \sa command_rw_callback
  */
int resistors_short_set( void )
{
  return command_rw_callback( short_read, short_write,
                              address_res_check, data_check );
};

/*@}*/
