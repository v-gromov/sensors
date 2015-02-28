/*
 * This file is part of the libopenstm32 project.
 *
 * Copyright (C) 2009 Federico Ruiz-Ugalde <memeruiz at gmail dot com>
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
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


#include "rcc.h"
#include "flash.h"

struct clock_values clocks = {
	8000000,	//sysclk
	40000,		//rtcclk
	8000000,	//ahbclk
	8000000,	//pclk1
	8000000,	//pclk2
	8000000,	//timclk1
	8000000,	//timclk2
	4000000		//adcclk
};

void rcc_osc_ready_int_clear(osc_t osc)
{
	switch (osc) {
	case PLL:
		RCC_CIR |= PLLRDYC;
		break;
	case PLL2:
		RCC_CIR |= PLL2RDYC;
		break;
	case HSE:
		RCC_CIR |= HSERDYC;
		break;
	case HSI:
		RCC_CIR |= HSIRDYC;
		break;
	case LSE:
		RCC_CIR |= LSERDYC;
		break;
	case LSI:
		RCC_CIR |= LSIRDYC;
		break;
	}
}

void rcc_osc_ready_int_enable(osc_t osc)
{
	switch (osc) {
	case PLL:
		RCC_CIR |= PLLRDYIE;
		break;
	case PLL2:
		RCC_CIR |= PLL2RDYIE;
		break;
	case HSE:
		RCC_CIR |= HSERDYIE;
		break;
	case HSI:
		RCC_CIR |= HSIRDYIE;
		break;
	case LSE:
		RCC_CIR |= LSERDYIE;
		break;
	case LSI:
		RCC_CIR |= LSIRDYIE;
		break;
	}
}

void rcc_osc_ready_int_disable(osc_t osc)
{
	switch (osc) {
	case PLL:
		RCC_CIR &= ~PLLRDYIE;
		break;
	case PLL2:
		RCC_CIR &= ~PLL2RDYIE;
		break;
	case HSE:
		RCC_CIR &= ~HSERDYIE;
		break;
	case HSI:
		RCC_CIR &= ~HSIRDYIE;
		break;
	case LSE:
		RCC_CIR &= ~LSERDYIE;
		break;
	case LSI:
		RCC_CIR &= ~LSIRDYIE;
		break;
	}
}

int rcc_osc_ready_int_flag(osc_t osc)
{
	switch (osc) {
	case PLL:
		return ((RCC_CIR & PLLRDYF) != 0);
		break;
	case PLL2:
		return ((RCC_CIR & PLL2RDYF) != 0);
		break;
	case HSE:
		return ((RCC_CIR & HSERDYF) != 0);
		break;
	case HSI:
		return ((RCC_CIR & HSIRDYF) != 0);
		break;
	case LSE:
		return ((RCC_CIR & LSERDYF) != 0);
		break;
	case LSI:
		return ((RCC_CIR & LSIRDYF) != 0);
		break;
	}

	/* Shouldn't be reached. */
	return -1;
}

void rcc_css_int_clear(void)
{
	RCC_CIR |= CSSC;
}

int rcc_css_int_flag(void)
{
	return ((RCC_CIR & CSSF) != 0);
}

void rcc_wait_for_osc_ready(osc_t osc)
{
	switch (osc) {
	case PLL:
		while ((RCC_CR & PLLRDY) == 0);
		break;
	case PLL2:
		while ((RCC_CR & PLL2RDY) == 0);
		break;
	case HSE:
		while ((RCC_CR & HSERDY) == 0);
		break;
	case HSI:
		while ((RCC_CR & HSIRDY) == 0);
		break;
	case LSE:
		while ((RCC_BDCR & LSERDY) == 0);
		break;
	case LSI:
		while ((RCC_CSR & LSIRDY) == 0);
		break;
	}
}

void rcc_osc_on(osc_t osc)
{
	switch (osc) {
	case PLL:
		RCC_CR |= PLLON;
		break;
	case PLL2:
		RCC_CR |= PLL2ON;
		break;
	case HSE:
		RCC_CR |= HSEON;
		break;
	case HSI:
		RCC_CR |= HSION;
		break;
	case LSE:
		RCC_BDCR |= LSEON;
		break;
	case LSI:
		RCC_CSR |= LSION;
		break;
	}
}

void rcc_osc_off(osc_t osc)
{
	switch (osc) {
	case PLL:
		RCC_CR &= ~PLLON;
		break;
	case PLL2:
		RCC_CR &= ~PLL2ON;
		break;
	case HSE:
		RCC_CR &= ~HSEON;
		break;
	case HSI:
		RCC_CR &= ~HSION;
		break;
	case LSE:
		RCC_BDCR &= ~LSEON;
		break;
	case LSI:
		RCC_CSR &= ~LSION;
		break;
	}
}

void rcc_css_enable(void)
{
	RCC_CR |= CSSON;
}

void rcc_css_disable(void)
{
	RCC_CR &= ~CSSON;
}

void rcc_osc_bypass_enable(osc_t osc)
{
	switch (osc) {
	case HSE:
		RCC_CR |= HSEBYP;
		break;
	case LSE:
		RCC_BDCR |= LSEBYP;
		break;
	case PLL:
	case PLL2:
	case HSI:
	case LSI:
		/* Do nothing, only HSE/LSE allowed here. */
		break;
	}
}

void rcc_osc_bypass_disable(osc_t osc)
{
	switch (osc) {
	case HSE:
		RCC_CR &= ~HSEBYP;
		break;
	case LSE:
		RCC_BDCR &= ~LSEBYP;
		break;
	case PLL:
	case PLL2:
	case HSI:
	case LSI:
		/* Do nothing, only HSE/LSE allowed here. */
		break;
	}
}

void rcc_peripheral_enable_clock(volatile u32 *reg, u32 en)
{
	*reg |= en;
}

void rcc_peripheral_disable_clock(volatile u32 *reg, u32 en)
{
	*reg &= ~en;
}

void rcc_peripheral_reset(volatile u32 *reg, u32 reset)
{
	*reg |= reset;
}

void rcc_peripheral_clear_reset(volatile u32 *reg, u32 clear_reset)
{
	*reg &= ~clear_reset;
}

/** \todo set up the clock tree in structure
  */
void rcc_set_sysclk_source(u32 clk)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 1) | (1 << 0));
	RCC_CFGR = (reg32 | clk);
}

void rcc_set_pll2_multiplication_factor(u32 mul)
{
	u32 reg32;
	reg32 = RCC_CFGR2;
	reg32 &= ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
	RCC_CFGR2 = (reg32 | (mul << 8 ));

}

void rcc_set_prediv1_clock_source( u32 source )
{
	u32 reg32;

	reg32 = RCC_CFGR2;
	reg32 &= ~(1 << 16);
	RCC_CFGR2 = (reg32 | (source << 16 ));
}

void rcc_set_prediv1_division_factor(u32 div)
{
	u32 reg32;

	reg32 = RCC_CFGR2;
	reg32 &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	RCC_CFGR2 = (reg32 | div << 0 );
}

void rcc_set_prediv2_division_factor(u32 div)
{
	u32 reg32;

	reg32 = RCC_CFGR2;
	reg32 &= ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
	RCC_CFGR2 = (reg32 | (div << 4));

}
void rcc_set_pll_multiplication_factor(u32 mul)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 21) | (1 << 20) | (1 << 19) | (1 << 18));
	RCC_CFGR = (reg32 | (mul << 18));
}

void rcc_set_pll_source(u32 pllsrc)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~(1 << 16);
	RCC_CFGR = (reg32 | (pllsrc << 16));
}

void rcc_set_pllxtpre(u32 pllxtpre)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~(1 << 17);
	RCC_CFGR = (reg32 | (pllxtpre << 17));
}

void rcc_set_adcpre(u32 adcpre)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 14) | (1 << 15));
	RCC_CFGR = (reg32 | (adcpre << 14));
}

void rcc_set_ppre2(u32 ppre2)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 11) | (1 << 12) | (1 << 13));
	RCC_CFGR = (reg32 | (ppre2 << 11));
}

void rcc_set_ppre1(u32 ppre1)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 8) | (1 << 9) | (1 << 10));
	RCC_CFGR = (reg32 | (ppre1 << 8));
}

void rcc_set_hpre(u32 hpre)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
	RCC_CFGR = (reg32 | (hpre << 4));
}

void rcc_set_usbpre(u32 usbpre)
{
	u32 reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~(1 << 22);
	RCC_CFGR = (reg32 | (usbpre << 22));
}

u32 rcc_system_clock_source(void)
{
	/* Return the clock source which is used as system clock. */
	return ((RCC_CFGR & 0x000c) >> 2);
}

/*
 * These functions are setting up the whole clock system for the most common
 * input clock and output clock configurations.
 */
void rcc_clock_setup_in_hsi_out_64mhz(void)
{
	/* Enable internal high-speed oscillator. */
	rcc_osc_on(HSI);
	rcc_wait_for_osc_ready(HSI);

	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSICLK);

	/*
	 * Set prescalers for AHB, ADC, ABP1, ABP2.
	 * Do this before touching the PLL (TODO: why?).
	 */
	rcc_set_hpre(HPRE_SYSCLK_NODIV);           /* Max. 72MHz */
	rcc_set_adcpre(ADCPRE_PLCK2_DIV8);         /* Max. 14MHz */
	rcc_set_ppre1(PPRE1_HCLK_DIV2);            /* Max. 36MHz */


rcc_set_ppre2(PPRE2_HCLK_NODIV);	/* Max. 72MHz */
	clocks=(struct clock_values){
		64000000,
		40000,
		64000000,
		32000000,
		64000000,
		64000000,
		64000000,
		8000000,
	};

	/*
	 * Sysclk is running with 64MHz -> 2 waitstates.
	 * 0WS from 0-24MHz
	 * 1WS from 24-48MHz
	 * 2WS from 48-72MHz
	 */
	flash_set_ws(FLASH_LATENCY_2WS);

	/*
	 * Set the PLL multiplication factor to 16.
	 * 8MHz (internal) * 16 (multiplier) / 2 (PLLSRC_HSI_CLK_DIV2) = 64MHz
	 */
	rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL16);

	/* Select HSI/2 as PLL source. */
	rcc_set_pll_source(PLLSRC_HSI_CLK_DIV2);

	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(PLL);
	rcc_wait_for_osc_ready(PLL);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_PLLCLK);
}

typedef enum {
  OUT_FREQ_48MHZ,
  OUT_FREQ_56MHZ,
  OUT_FREQ_72MHZ,
} out_freq_t;
#if(0)
static void rcc_clock_setup_in_hse_25mhz( out_freq_t out_freq )
{
	/* Enable internal high-speed oscillator. */
	rcc_osc_on(HSI);
	rcc_wait_for_osc_ready(HSI);

	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSICLK);

	/* Enable external high-speed oscillator 8MHz. */
	rcc_osc_on(HSE);
	rcc_wait_for_osc_ready(HSE);
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSECLK);
	
	/*
	 * Set prescalers for AHB, ADC, ABP1, ABP2.
	 * Do this before touching the PLL (TODO: why?).
	 */
	rcc_set_hpre(HPRE_SYSCLK_NODIV);	/* Max. 72MHz */
	rcc_set_adcpre(ADCPRE_PLCK2_DIV8);	/* Max. 14MHz */
	rcc_set_ppre1(PPRE1_HCLK_DIV2);		/* Max. 36MHz */
	rcc_set_ppre2(PPRE2_HCLK_NODIV);	/* Max. 72MHz */
	
	/*
	 * Sysclk runs with 72MHz -> 2 waitstates.
	 * 0WS from 0-24MHz
	 * 1WS from 24-48MHz
	 * 2WS from 48-72MHz
	 */
	flash_set_ws(FLASH_LATENCY_2WS);
	rcc_set_usbpre( USBPRE_PLL_VCO_CLK_DIV3 ); /* 48MHz */
	/*setup prediv2*/
	rcc_set_prediv2_division_factor(PREDIV2_DIV5);
	/*setup ppl2mul*/
	rcc_set_pll2_multiplication_factor(PLL2MUL_PLL2_CLK_MUL8);
	rcc_osc_on(PLL2);
	rcc_wait_for_osc_ready(PLL2);
	/*setup prediv1scr*/
	rcc_set_prediv1_clock_source( PREDIV1SRC_PLL2_CLK );
	/*setup prediv1*/
	rcc_set_prediv1_division_factor( PREDIV2_DIV5 );

	// now we have 8Mhz on PLLMUL input

        switch( out_freq ) {
	  case OUT_FREQ_48MHZ:
	    rcc_set_usbpre( USBPRE_PLL_VCO_CLK_DIV2 ); /* 48MHz */
	    /*
	     * Set the PLL multiplication factor to 6.
	     * 8MHz (external) * 6 (multiplier) = 48MHz
	     */
	    rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL6);
	    clocks=(struct clock_values){
	      48000000,
	      40000,
	      48000000,
	      24000000,
	      48000000,
	      48000000,
	      48000000,
	      6000000,
	    };
	    flash_set_ws(FLASH_LATENCY_1WS);
	    break;
          case OUT_FREQ_56MHZ:
            /*
             * Set the PLL multiplication factor to 7.
             * 8MHz (external) * 7 (multiplier) = 56MHz
             */
            rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL7);
	    clocks=(struct clock_values){
		56000000,
		40000,
		56000000,
		28000000,
		56000000,
		56000000,
		56000000,
		7000000,
	    }; 
	    flash_set_ws(FLASH_LATENCY_2WS);
            break;
	  case OUT_FREQ_72MHZ:
	    /*
	     * Set the PLL multiplication factor to 9.
	     * 8MHz (external) * 9 (multiplier) = 72MHz
	     */
	    rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL9);
	    clocks=(struct clock_values){
	      72000000,
	      40000,
	      72000000,
	      36000000,
	      72000000,
	      72000000,
	      72000000,
	      9000000,
	    };
	    flash_set_ws(FLASH_LATENCY_2WS);
	    break;
        }

	/* Select HSE as PLL source. */
	rcc_set_pll_source(PLLSRC_HSE_CLK);

	/*
	 * External frequency undivided before entering PLL
	 * (only valid/needed for HSE).
	 */
	rcc_set_pllxtpre(PLLXTPRE_HSE_CLK);

	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(PLL);
	rcc_wait_for_osc_ready(PLL);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_PLLCLK);
}
#endif
static void rcc_clock_setup_in_hse_8mhz( out_freq_t out_freq )
{
	/* Enable internal high-speed oscillator. */
	rcc_osc_on(HSI);
	rcc_wait_for_osc_ready(HSI);

	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSICLK);

	/* Enable external high-speed oscillator 8MHz. */
	rcc_osc_on(HSE);
	rcc_wait_for_osc_ready(HSE);
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSECLK);

	/*
	 * Set prescalers for AHB, ADC, ABP1, ABP2.
	 * Do this before touching the PLL (TODO: why?).
	 */
	rcc_set_hpre(HPRE_SYSCLK_NODIV);	/* Max. 72MHz */
	rcc_set_adcpre(ADCPRE_PLCK2_DIV8);	/* Max. 14MHz */
	rcc_set_ppre1(PPRE1_HCLK_DIV2);		/* Max. 36MHz */
	rcc_set_ppre2(PPRE2_HCLK_NODIV);	/* Max. 72MHz */

	/*
	 * Sysclk runs with 72MHz -> 2 waitstates.
	 * 0WS from 0-24MHz
	 * 1WS from 24-48MHz
	 * 2WS from 48-72MHz
	 */

        switch( out_freq ) {
          case OUT_FREQ_48MHZ:
	    rcc_set_usbpre( USBPRE_PLL_VCO_CLK_DIV2 ); /* 48MHz */
            /*
             * Set the PLL multiplication factor to 6.
             * 8MHz (external) * 6 (multiplier) = 48MHz
             */
            rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL6);
	clocks=(struct clock_values){
		48000000,
		40000,
		48000000,
		24000000,
		48000000,
		48000000,
		48000000,
		6000000,
	};
	flash_set_ws(FLASH_LATENCY_1WS);
            break;
          case OUT_FREQ_56MHZ:
            /*
             * Set the PLL multiplication factor to 9.
             * 8MHz (external) * 7 (multiplier) = 56MHz
             */
            rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL7);
	clocks=(struct clock_values){
		56000000,
		40000,
		56000000,
		28000000,
		56000000,
		56000000,
		56000000,
		7000000,
	};
	flash_set_ws(FLASH_LATENCY_2WS);
            break;
          case OUT_FREQ_72MHZ:
	    rcc_set_usbpre( USBPRE_PLL_VCO_CLK_DIV3 ); /* 48MHz */
            /*
             * Set the PLL multiplication factor to 9.
             * 8MHz (external) * 9 (multiplier) = 72MHz
             */
            rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL9);
	clocks=(struct clock_values){
		72000000,
		40000,
		72000000,
		36000000,
		72000000,
		72000000,
		72000000,
		9000000,
	};
	flash_set_ws(FLASH_LATENCY_2WS);
            break;
        }



	/* Select HSE as PLL source. */
	rcc_set_pll_source(PLLSRC_HSE_CLK);

	/*
	 * External frequency undivided before entering PLL
	 * (only valid/needed for HSE).
	 */
	rcc_set_pllxtpre(PLLXTPRE_HSE_CLK);

	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(PLL);
	rcc_wait_for_osc_ready(PLL);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_PLLCLK);

}

void rcc_clock_setup_in_hse_8mhz_out_72mhz(void)
{
  rcc_clock_setup_in_hse_8mhz( OUT_FREQ_72MHZ );
}

/*void rcc_clock_setup_in_hse_25mhz_out_72mhz(void)
{
  rcc_clock_setup_in_hse_25mhz( OUT_FREQ_72MHZ );
}*/

void rcc_clock_setup_in_hse_8mhz_out_48mhz(void)
{
  rcc_clock_setup_in_hse_8mhz( OUT_FREQ_48MHZ );
}

void rcc_clock_setup_in_hse_8mhz_out_56mhz(void)
{
  rcc_clock_setup_in_hse_8mhz( OUT_FREQ_56MHZ );
}


/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
//  RCC_CFGR|=(1<<22);//Divide clock by 1

  /* Enable the USB clock */ 
  RCC_AHBENR |= OTGEN;
}


/*
void rcc_set_pll1_multiplication_factor(u32 mul)
{
        u32 reg32;
 
        reg32 = RCC_CFGR;
        reg32 &= ~((1 << 21) | (1 << 20) | (1 << 19) | (1 << 18));
        RCC_CFGR = (reg32 | (mul << 18));
}
 
void rcc_set_pll2_multiplication_factor(u32 mul)
{
        u32 reg32;
 
        reg32 = RCC_CFGR2;
        reg32 &= ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
        RCC_CFGR2 = (reg32 | (mul << 8));
}
 
void rcc_set_pre1_div_factor(u32 div) 
{
        u32 reg32;
 
        reg32 = RCC_CFGR2;
        reg32 &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
        RCC_CFGR2 = (reg32 | div);
}
 
void rcc_set_pre2_div_factor(u32 div)
{
        u32 reg32;
 
        reg32 = RCC_CFGR2;
        reg32 &= ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
        RCC_CFGR2 = (reg32 | (div << 4));
}
*/


 #define  RCC_CFGR2_PREDIV2                  ((uint32_t)0x000000F0)        /*!< PREDIV2[3:0] bits */
 #define  RCC_CFGR2_PREDIV2_DIV5             ((uint32_t)0x00000040)        /*!< PREDIV2 input clock divided by 5 */
 #define  RCC_CFGR2_PLL2MUL                  ((uint32_t)0x00000F00)        /*!< PLL2MUL[3:0] bits */
 #define  RCC_CFGR2_PLL2MUL8                 ((uint32_t)0x00000600)        /*!< PLL2 input clock * 8 */
 #define  RCC_CFGR2_PREDIV1                  ((uint32_t)0x0000000F)        /*!< PREDIV1[3:0] bits */
 #define  RCC_CFGR2_PREDIV1SRC               ((uint32_t)0x00010000)        /*!< PREDIV1 entry clock source */
 #define  RCC_CFGR2_PREDIV1SRC_PLL2          ((uint32_t)0x00010000)        /*!< PLL2 selected as PREDIV1 entry clock source */
 #define  RCC_CFGR2_PREDIV1_DIV5             ((uint32_t)0x00000004)        /*!< PREDIV1 input clock divided by 5 */
 #define  RCC_CR_PLL2ON                       ((uint32_t)0x04000000)        /*!< PLL2 enable */
 #define  RCC_CR_PLL2RDY                      ((uint32_t)0x08000000)        /*!< PLL2 clock ready flag */
 #define  RCC_CFGR_PLLXTPRE                   ((uint32_t)0x00020000)        /*!< HSE divider for PLL entry */
 #define  RCC_CFGR_PLLXTPRE_PREDIV1          ((uint32_t)0x00000000)        /*!< PREDIV1 clock not divided for PLL entry */
 #define  RCC_CFGR_PLLSRC_PREDIV1            ((uint32_t)0x00010000)        /*!< PREDIV1 clock selected as PLL entry clock source */
 #define  RCC_CFGR_PLLMULL9                  ((uint32_t)0x001C0000)        /*!< PLL input clock * 9 */
 #define  RCC_CFGR_PLLSRC                     ((uint32_t)0x00010000)        /*!< PLL entry clock source */
 #define  RCC_CFGR_PLLMULL                    ((uint32_t)0x003C0000)        /*!< PLLMUL[3:0] bits (PLL multiplication factor) */
 
void rcc_clock_setup_in_hse_25mhz_out_72mhz(void)
{
	/* Enable internal high-speed oscillator. */
	rcc_osc_on(HSI);
	rcc_wait_for_osc_ready(HSI);

	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSICLK);

	/* Enable external high-speed oscillator 8MHz. */
	rcc_osc_on(HSE);
	rcc_wait_for_osc_ready(HSE);
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSECLK);

	/*
	 * Set prescalers for AHB, ADC, ABP1, ABP2.
	 * Do this before touching the PLL (TODO: why?).
	 */
	rcc_set_hpre(HPRE_SYSCLK_NODIV);	/* Max. 72MHz */
	rcc_set_adcpre(ADCPRE_PLCK2_DIV8);	/* Max. 14MHz */
	rcc_set_ppre1(PPRE1_HCLK_DIV2);		/* Max. 36MHz */
	rcc_set_ppre2(PPRE2_HCLK_NODIV);	/* Max. 72MHz */
	rcc_set_usbpre( USBPRE_PLL_VCO_CLK_DIV3 ); /* 48MHz */
	
    RCC_CFGR2 &= (uint32_t)~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
                              RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
    RCC_CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
                             RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);
  
    /* Enable PLL2 */
    RCC_CR |= RCC_CR_PLL2ON;
    /* Wait till PLL2 is ready */
    while((RCC_CR & RCC_CR_PLL2RDY) == 0)
    {
    }
    
   
    /* PLL configuration: PLLCLK = PREDIV1 * 9 = 72 MHz */ 
    RCC_CFGR &= (uint32_t)~(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC_CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 | 
                            RCC_CFGR_PLLMULL9); 
			
			
			
			

	flash_set_ws(FLASH_LATENCY_2WS);




	/* Select HSE as PLL source. */
	rcc_set_pll_source(PLLSRC_HSE_CLK);

	/*
	 * External frequency undivided before entering PLL
	 * (only valid/needed for HSE).
	 */
	rcc_set_pllxtpre(PLLXTPRE_HSE_CLK);

	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(PLL);
	rcc_wait_for_osc_ready(PLL);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_PLLCLK);
}
