/*
 * This file is part of the libopenstm32 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "usart.h"
#include "rcc.h"

void usart_set_baudrate(u32 usart, u32 baud)
{
  	u32 clock=clocks.pclk2;
        if(usart==USART2){
	clock = clocks.pclk1; /* FIXME: Don't hardcode this clock! */
	};
	

	/* TODO: Document and explain calculation. */
	USART_BRR(usart) = (u16)((clock << 4) / (baud * 16));
}

void usart_set_databits(u32 usart, u32 bits)
{
	if (bits == 8)
		USART_CR1(usart) &= ~USART_CR1_M; /* 8 data bits */
	else
		USART_CR1(usart) |= USART_CR1_M;  /* 9 data bits */
}

void usart_set_stopbits(u32 usart, u32 stopbits)
{
	u32 reg32;

	reg32 = USART_CR2(usart);
	reg32 = (reg32 & ~((1 << 13) | (1 << 12))) | (stopbits << 12);
	USART_CR2(usart) = reg32;
}

void usart_set_parity(u32 usart, u32 parity)
{
	u32 reg32;

	reg32 = USART_CR1(usart);
	reg32 = (reg32 & ~((1 << 10) | (1 << 9))) | (parity << 9);
	USART_CR1(usart) = reg32;
}

void usart_set_mode(u32 usart, u32 mode)
{
	u32 reg32;

	reg32 = USART_CR1(usart);
	reg32 = (reg32 & ~((1 << 3) | (1 << 2))) | (mode << 2);
	USART_CR1(usart) = reg32;
}

void usart_set_flow_control(u32 usart, u32 flowcontrol)
{
	u32 reg32;

	reg32 = USART_CR3(usart);
	reg32 = (reg32 & ~((1 << 9) | (1 << 8))) | (flowcontrol << 2);
	USART_CR3(usart) = reg32;
}

void usart_enable(u32 usart)
{
	USART_CR1(usart) |= USART_CR1_UE;
}

void usart_disable(u32 usart)
{
	USART_CR1(usart) &= ~USART_CR1_UE;
}

void usart_send(u32 usart, u16 data)
{
	/* Send data. */
	USART_DR(usart) = (data & 0x1ff);

	/* Wait until the data has been transferred into the shift register. */
	while ((USART_SR(usart) & USART_SR_TXE) == 0);
}

u16 usart_recv(u32 usart)
{
	/* Wait until the data is ready to be received. */
	while ((USART_SR(usart) & USART_SR_RXNE) == 0);

	/* Receive data. */
	return USART_DR(usart) & 0x1ff;
}

int usart_rxne(u32 usart)
{
	return ((USART_SR(usart) & USART_SR_RXNE));
};
