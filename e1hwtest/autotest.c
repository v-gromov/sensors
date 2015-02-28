/** \file autotest.c
  * \brief Automatic tests for the E1 card
  */
#include <stdio.h>

#include <common/fpga.h>
#include <common/service.h>
#include <common/i2c.h>
#include <common/io.h>
#include <common/power.h>
#include <common/autotests.h>

#include "pca9532.h"
#include "fpgaregs.h"

/** \brief Runs whole set of automatic tests: Serial console, Power on/off, Leds, 
  * fpga firmware upload, fpga register access, liu register access, codec access,
  * then shuts down the board power
  */
void autotests( void )
{
  int number = 1;               //test number
  int i, flag;
  //Heading
//  printf( "%3s|%25s|%10s\n\r", "N", "Test", "Result" );
//  printf( "---+-------------------------+----------\n\r" );
  //End of heading
  TEST_RESULT( "Serial console", 1 );

  power_on( );
  TEST_RESULT( "Board power", power_get( ) );

  TwInit(0);

  Pca9532Init(  );
  flag = 1;
  for( i = 0; i < 8; i++ ) {
    int value;
    Pca9532SetBitLow( i );
    Pca9532GetBit( i, &value );
    if( value != 0 )
      flag = 0;
  };

  b5_set( CARD_ERR_LED );
  if( !b5_get( CARD_ERR_LED ) ) {
    flag = 0;
  }
  b5_set( CARD_PWR_LED );
  if( !b5_get( CARD_PWR_LED ) ) {
    flag = 0;
  }

  TEST_RESULT( "Leds", flag );

  TEST_RESULT( "DataFlash -> FPGA", !df2fpga(  ) );

  TEST_RESULT( "FPGA Register access", !fpga_reg_autotest( 0x03 ) );//FIXME - remove hardcoded address

  TEST_RESULT( "LIU Registers access", flag );

  TEST_RESULT( "CODEC Registers access", flag );

  //Shut down the board
  power_off( );
  puts( "\r" );
};
