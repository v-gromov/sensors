/** \file codec.c
  * \brief This file contains functions for access and control of codec
  */

#include <stdio.h>

#include <common/service.h>
#include <common/i2c.h>

#include "gpio.h"
#include "codec.h"

/** Resets codec and then puts it out of reset
  *
  */
void codec_init( void )
{
  b5_clear( CARD_CODEC_RST );
  b5_set( CARD_CODEC_RST );
  //Enable sidetone, mic1,ear1,tx enable, rx enable,micbias enable
  codec_write( 0x00, 0x1b ); //FIXME: hardcode
};

/** Read codec (TLV320AIC1110) register
  * \sa command_rw_callback()
  */
int codec_read( int addr )
{
  int d;
  TwMasterRegRead( TLV320AIC_ADDR, addr, 1, &d, 1 );
  return d;
};

/** Write codec (TLV320AIC1110) register
  * \sa command_rw_callback()
  */
int codec_write( int addr, int data )
{
  TwMasterRegWrite( TLV320AIC_ADDR, addr, 1, &data, 1 );
  return data;
};

/** \sa command_rw_callback()
  */
static int address_check( int address )
{
  if( address < 0 )
    return -1;
  if( address > 6 )
    return -1;
  return 0;
};

/** \sa command_rw_callback()
  */
static int data_check( int data )
{
  if( data < 0 )
    return -1;
  if( data > 256 )
    return -1;
  return 0;
};

/** \sa command_rw_callback()
  */
int codec_rw( void )
{
  return command_rw_callback( codec_read, codec_write, address_check,
                              data_check );
};
