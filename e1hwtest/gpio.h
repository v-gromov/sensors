#ifndef GPIO__H
#define GPIO__H

#include <libopenstm32/gpio.h>

#define SENSOR1_BIT		GPIO14
#define SENSOR1_PORT		GPIOD

#define SENSOR2_BIT		GPIO15
#define SENSOR2_PORT		GPIOD

#define SENSOR3_BIT		GPIO8
#define SENSOR3_PORT		GPIOC

#define SENSOR4_BIT		GPIO9
#define SENSOR4_PORT		GPIOC

#define ONBOARD_LED_BIT		GPIO15
#define ONBOARD_LED_PORT	GPIOE

/// pin means the pin name without _PORT and _BIT suffixes.
/// they are appended automatically.
/// Note, that this function is usable only when you 
/// need to set mode for single pin. Otherwise, you
/// should use gpio_set_mode()
/// \sa gpio_set_mode()
#define b5_set_mode( pin, mode, cnf ) \
  gpio_set_mode( pin ## _PORT, mode, cnf, pin ## _BIT )

#define b5_set( pin )  gpio_set( pin ## _PORT, pin ## _BIT )
#define b5_clear( pin )  gpio_clear( pin ## _PORT, pin ## _BIT )
#define b5_get( pin )  gpio_get( pin ## _PORT, pin ## _BIT )

void common_gpio_setup( void );
void gpio_setup( void );

#endif
