#ifndef GPIO__H
#define GPIO__H

#include <libopenstm32/gpio.h>

#define CARD_US3_CLK_BIT  	GPIO10
#define CARD_US3_CLK_PORT	GPIOD

#define CARD_US3_RX_BIT  	GPIO9
#define CARD_US3_RX_PORT	GPIOD

#define CARD_US3_TX_BIT  	GPIO8
#define CARD_US3_TX_PORT	GPIOD

#define CARD_LIU_CS_BIT		GPIO13
#define CARD_LIU_CS_PORT	GPIOD

#define CARD_LIU_RST_BIT	GPIO4
#define CARD_LIU_RST_PORT	GPIOD

#define CARD_CODEC_RST_BIT	GPIO0
#define CARD_CODEC_RST_PORT	GPIOE


#define CARD_POWER_BIT	GPIO0
#define CARD_POWER_PORT	GPIOD

#define CARD_NCONFIG_BIT	GPIO15
#define CARD_NCONFIG_PORT	GPIOE

#define CARD_CONF_DONE_BIT	GPIO14
#define CARD_CONF_DONE_PORT	GPIOE

#define CARD_NSTATUS_BIT	GPIO14
#define CARD_NSTATUS_PORT	GPIOB

#define CARD_CRC_ERR_BIT	GPIO12
#define CARD_CRC_ERR_PORT	GPIOB

#define CARD_USART2_TX_BIT	GPIO5
#define CARD_USART2_TX_PORT	GPIOD

#define CARD_USART2_RX_BIT	GPIO6
#define CARD_USART2_RX_PORT	GPIOD

#define CARD_PWR_LED_BIT	GPIO4
#define CARD_PWR_LED_PORT	GPIOE

#define CARD_ERR_LED_BIT	GPIO3
#define CARD_ERR_LED_PORT	GPIOE

//SPI1
#define CARD_SPI1_NSS_BIT	GPIO4
#define CARD_SPI1_NSS_PORT	GPIOA

#define CARD_SPI1_MISO_BIT	GPIO6
#define CARD_SPI1_MISO_PORT	GPIOA

#define CARD_SPI1_MOSI_BIT	GPIO7
#define CARD_SPI1_MOSI_PORT	GPIOA

#define CARD_SPI1_SCK_BIT	GPIO5
#define CARD_SPI1_SCK_PORT	GPIOA

//SPI2
#define CARD_SPI2_NSS_BIT	GPIO12
#define CARD_SPI2_NSS_PORT	GPIOB

#define CARD_SPI2_MISO_BIT	GPIO14
#define CARD_SPI2_MISO_PORT	GPIOB

#define CARD_SPI2_MOSI_BIT	GPIO15
#define CARD_SPI2_MOSI_PORT	GPIOB

#define CARD_SPI2_SCK_BIT	GPIO13
#define CARD_SPI2_SCK_PORT	GPIOB

//SPI3
#define CARD_SPI3_NSS_BIT	GPIO2
#define CARD_SPI3_NSS_PORT	GPIOD

#define CARD_SPI3_MISO_BIT	GPIO11
#define CARD_SPI3_MISO_PORT	GPIOC

#define CARD_SPI3_MOSI_BIT	GPIO12
#define CARD_SPI3_MOSI_PORT	GPIOC

#define CARD_SPI3_SCK_BIT	GPIO10
#define CARD_SPI3_SCK_PORT	GPIOC

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
