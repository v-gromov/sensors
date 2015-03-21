#ifndef GPIO__H
#define GPIO__H

#include "io.h"

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

void gpio_setup( void );

#endif
