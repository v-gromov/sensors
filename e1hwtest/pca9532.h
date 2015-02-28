#ifndef PCA9532__H
#define PCA9532__H

/*!
 * \addtogroup LibCommon
 */

/*@{*/


#define PCA9532_ADDR     0xC0

#define PCA_INPUT0      0
#define PCA_INPUT1      1
#define PCA_PSC0        2
#define PCA_PWM0        3
#define PCA_PSC1        4
#define PCA_PWM1        5
#define PCA_LS0         6
#define PCA_LS1         7
#define PCA_LS2         8
#define PCA_LS3         9
#define PCA_AUTOINC     16

#define MAX_LED	7
#define MAX_RES 3

//States of pins controlling LEDs and resistors
//  State low: for LEDs - LED is ON; for short resistors - resistor is OFF;
//  for load resistors - resistor is ON.
#define STATE_LOW                  (0x1)
//  State high: for LEDs - LED is OFF; for short resistors - resistor is ON;
//  for load resistors - resistor is OFF.
#define STATE_HIGH                 (0x0)
//  For LEDs only: LED blinks at PWM0 rate
#define STATE_BLINK_PWM0           (0x2)
//  For LEDs only: LED blinks at PWM1 rate
#define STATE_BLINK_PWM1           (0x3)

//A mask used to set a specified device (LED or resistor) to specified state
#define PCA_SET_STATE( reg, led, st ) \
  do { \
    unsigned char reg_mask = 0x03 << ( ( led % 4 ) * 2 ); \
    reg &= ~reg_mask; \
    reg |= st << ( ( led % 4 ) * 2 ); \
  } while(0) 

int Pca9532GetBit( int bit, int *value );
int Pca9532SetBitHigh( int bit );
int Pca9532SetBitLow( int bit );
int Pca9532Init( void );
int short_write( int address, int data );
int leds_set( void );
int resistors_load_set( void );
int resistors_short_set( void );

/*@}*/

#endif
