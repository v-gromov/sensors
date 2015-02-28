/*
 * This file is part of the libopenstm32 project.
 *
 * Copyright (C) 2009 Edward Cheeseman <evbuilder@users.sourceforge.net>
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

/*
 * Basic ADC handling API.
 *
 * Examples:
 *  rcc_peripheral_enable_clock(&RCC_APB2ENR, ADC1EN);
 *  rcc_peripheral_disable_clock(&RCC_APB2ENR, ADC1EN);
 *  rcc_peripheral_reset(&RCC_APB2RSTR, ADC1RST);
 *  rcc_peripheral_clear_reset(&RCC_APB2RSTR, ADC1RST);
 *
 *  rcc_set_adc_clk(ADC_PRE_PLCK2_DIV2);
 *  adc_set_mode(ADC1, TODO);
 *  reg16 = adc_read(ADC1, ADC_CH_0);
 */

#include "rcc.h"
#include "adc.h"

/** Initialize ADC to work with cpecified channel.
 *
 * TODO: break this function for more. Currently this function 
 *       is very greedy for time. It repeats 'nop's many times.
 */
void adc_init(u8 channel)
{
  int i;
  u8 channel_array[16];
  //������������������� ADC
  rcc_peripheral_enable_clock(&RCC_APB2ENR, ADC1EN);

  /* Make shure it doesn't run during config. */
  adc_off(ADC1);

  /* We configure everything for one single conversion. */
  adc_disable_scan_mode(ADC1);
  adc_set_single_conversion_mode(ADC1);
  adc_enable_discontinous_mode_regular(ADC1);
  adc_disable_external_trigger_regular(ADC1);
  adc_set_right_aligned(ADC1);
  /* We want to read the temperature sensor, so we have to enable it. */
  adc_enable_temperature_sensor(ADC1);
  adc_set_conversion_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);

  adc_on(ADC1);

  /* Wait for ADC starting up. */
  for (i = 0; i < 800000; i++)    /* Wait a bit. */
    __asm__("nop");

  adc_reset_calibration(ADC1);
  adc_calibration(ADC1);

  //������� ����� ADC
  channel_array[0] = channel;
  adc_set_regular_sequence(ADC1, 1, channel_array);
}

/* \brief Function starts new conversion, waits for 
 *        it's completion and returns result.
 *        FIXME: For now it is assumed that 
 *        Note, that function loop while waiting for
 *        completion.
 * \details If the ADC_CR2_ON bit is already set -> setting it
 *          another time starts the conversion.
 * \return result of conversion         
 */
unsigned short adc_read( void ) 
{
  adc_on(ADC1);

  /* Wait for end of conversion. */
  while (!(ADC_SR(ADC1) & ADC_SR_EOC));

  return ADC_DR(ADC1);
}

void adc_enable_analog_watchdog_regular(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_AWDEN;
}

void adc_disable_analog_watchdog_regular(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_AWDEN;
}

void adc_enable_analog_watchdog_injected(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_JAWDEN;
}

void adc_disable_analog_watchdog_injected(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_JAWDEN;
}

void adc_enable_discontinous_mode_regular(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_DISCEN;
}

void adc_disable_discontinous_mode_regular(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_DISCEN;
}

void adc_enable_discontinous_mode_injected(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_JDISCEN;
}

void adc_disable_discontinous_mode_injected(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_JDISCEN;
}

void adc_enable_automatic_injected_group_conversion(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_JAUTO;
}

void adc_disable_automatic_injected_group_conversion(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_JAUTO;
}

void adc_enable_analog_watchdog_on_all_channels(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_AWDSGL;
}

void adc_enable_analog_watchdog_on_selected_channel(u32 adc, u8 channel)
{
	u32 reg32;

	reg32 = (ADC_CR1(adc) & 0xffffffe0); /* Clear bits [4:0]. */
	if (channel < 18)
		reg32 |= channel;
	ADC_CR1(adc) = reg32;
	ADC_CR1(adc) &= ~ADC_CR1_AWDSGL;
}

void adc_enable_scan_mode(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_SCAN;
}

void adc_disable_scan_mode(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_SCAN;
}

void adc_enable_jeoc_interrupt(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_JEOCIE;
}

void adc_disable_jeoc_interrupt(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_JEOCIE;
}

void adc_enable_awd_interrupt(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_AWDIE;
}

void adc_disable_awd_interrupt(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_AWDIE;
}

void adc_enable_eoc_interrupt(u32 adc)
{
	ADC_CR1(adc) |= ADC_CR1_EOCIE;
}

void adc_disable_eoc_interrupt(u32 adc)
{
	ADC_CR1(adc) &= ~ADC_CR1_EOCIE;
}

void adc_enable_temperature_sensor(u32 adc)
{
	ADC_CR2(adc) |= ADC_CR2_TSVREFE;
}

void adc_disable_temperature_sensor(u32 adc)
{
	ADC_CR2(adc) &= ~ADC_CR2_TSVREFE;
}

void adc_start_conversion_regular(u32 adc)
{
	/* start conversion on regular channels */
	ADC_CR2(adc) |= ADC_CR2_SWSTART;
	
	/* wait til the ADC starts the conversion */
	while (ADC_CR2(adc) & ADC_CR2_SWSTART);
}

void adc_start_conversion_injected(u32 adc)
{
	/* start conversion on injected channels */
	ADC_CR2(adc) |= ADC_CR2_JSWSTART;
	
	/* wait til the ADC starts the conversion */
	while (ADC_CR2(adc) & ADC_CR2_JSWSTART);
}

void adc_enable_external_trigger_regular(u32 adc, u8 trigger)
{
	u32 reg32;

	reg32 = (ADC_CR2(adc) & 0xfff1ffff); /* Clear bits [19:17]. */
	if (trigger < 8)
		reg32 |= (trigger << ADC_CR2_EXTSEL_LSB);
	ADC_CR2(adc) = reg32;
	ADC_CR2(adc) |= ADC_CR2_EXTTRIG;
}

void adc_disable_external_trigger_regular(u32 adc)
{
	ADC_CR2(adc) &= ~ADC_CR2_EXTTRIG;
}

void adc_enable_external_trigger_injected(u32 adc, u8 trigger)
{
	u32 reg32;

	reg32 = (ADC_CR2(adc) & 0xffff8fff); /* Clear bits [12:14]. */
	if (trigger < 8)
		reg32 |= (trigger << ADC_CR2_JEXTSEL_LSB);
	ADC_CR2(adc) = reg32;
	ADC_CR2(adc) |= ADC_CR2_JEXTTRIG;
}

void adc_disable_external_trigger_injected(u32 adc)
{
	ADC_CR2(adc) &= ~ADC_CR2_JEXTTRIG;
}

void adc_set_left_aligned(u32 adc)
{
	ADC_CR2(adc) |= ADC_CR2_ALIGN;
}

void adc_set_right_aligned(u32 adc)
{
	ADC_CR2(adc) &= ~ADC_CR2_ALIGN;
}

void adc_enable_dma(u32 adc)
{
	if ((adc == ADC1) | (adc==ADC3))
		ADC_CR2(adc) |= ADC_CR2_DMA;
}

void adc_disable_dma(u32 adc)
{
	if ((adc == ADC1) | (adc==ADC3))
		ADC_CR2(adc) &= ~ADC_CR2_DMA;
}
	
void adc_reset_calibration(u32 adc)
{
	ADC_CR2(adc) |= ADC_CR2_RSTCAL;
	while (ADC_CR2(adc) & ADC_CR2_RSTCAL);
}

void adc_calibration(u32 adc)
{
	ADC_CR2(adc) |= ADC_CR2_CAL;
	while (ADC_CR2(adc) & ADC_CR2_CAL);
}

void adc_set_continous_conversion_mode(u32 adc)
{
	ADC_CR2(adc) |= ADC_CR2_CONT;
}

void adc_set_single_conversion_mode(u32 adc)
{
	ADC_CR2(adc) &= ~ADC_CR2_CONT;
}

void adc_on(u32 adc)
{
	ADC_CR2(adc) |= ADC_CR2_ADON;
}

void adc_off(u32 adc)
{
	ADC_CR2(adc) &= ~ADC_CR2_ADON;
}

void adc_set_conversion_time(u32 adc, u8 channel, u8 time)
{
	u32 reg32;

	if (channel < 10) {
		reg32 = ADC_SMPR2(adc);
		reg32 &= ~(0b111 << (channel * 3));
		reg32 |= (time << (channel * 3));
		ADC_SMPR2(adc) = reg32;
	} 
	else {
		reg32 = ADC_SMPR1(adc);
		reg32 &= ~(0b111 << ((channel-10) * 3));
		reg32 |= (time << ((channel-10) * 3));
		ADC_SMPR1(adc) = reg32;
	}
}

void adc_set_conversion_time_on_all_channels(u32 adc, u8 time)
{
	u32 reg32 = 0;
	u8 i;

	for (i = 0; i <= 9; i++) {
		reg32 |= (time << (i * 3));
	}
	ADC_SMPR2(adc) = reg32;

	for (i = 10; i <= 17; i++) {
		reg32 |= (time << ((i-10) * 3));
	}
	ADC_SMPR1(adc) = reg32;
}

void adc_set_watchdog_high_threshold(u32 adc, u16 threshold)
{
	u32 reg32 = 0;

	reg32 = (u32)threshold;
	reg32 &= ~0xfffff000; /* clear all bits above 11 */
	ADC_HTR(adc) = reg32;
}

void adc_set_watchdog_low_threshold(u32 adc, u16 threshold)
{
	u32 reg32 = 0;

	reg32 = (u32)threshold;
	reg32 &= ~0xfffff000; /* clear all bits above 11 */
	ADC_LTR(adc) = reg32;
}

void adc_set_regular_sequence(u32 adc, u8 length, u8 channel[])
{
	u32 reg32_1 = 0;
	u32 reg32_2 = 0;
	u32 reg32_3 = 0;
	u8 i = 0;

	/* maximum sequence length is 16 channels */
	if (length > 16)
		return;

	for (i=1; i<=length; i++) {
		if (i <= 6) 
			reg32_3 |= (channel[i-1] << ((i-1) * 5));
		if ((i > 6) & (i <= 12))
			reg32_2 |= (channel[i-1] << ((i-6-1) * 5));
		if ((i > 12) & (i <= 16))
			reg32_1 |= (channel[i-1] << ((i-12-1) * 5));
	}
	reg32_1 |= ((length -1) << ADC_SQR1_L_LSB);

	ADC_SQR1(adc) = reg32_1;
	ADC_SQR2(adc) = reg32_2;
	ADC_SQR3(adc) = reg32_3;
}

void adc_set_injected_sequence(u32 adc, u8 length, u8 channel[])
{
	u32 reg32 = 0;
	u8 i = 0;

	/* maximum sequence length is 4 channels */
	if (length > 4)
		return;

	for (i = 1; i <= length; i++) {
		reg32 |= (channel[i-1] << ((i-1) * 5));
	}
	reg32 |= ((length-1) << ADC_JSQR_JL_LSB);

	ADC_JSQR(adc) = reg32;
}

