/** \brief Contains configuration constants for LibUsbSerial
  * \author Nikolaj Zamotaev
  * \author STMicroelectronics
  * \file usb_conf.c
  */

/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_conf.c
* Author             : MCD Application Team
* Version            : V3.1.1
* Date               : 04/07/2010
* Description        : Virtual COM Port Demo configuration  header
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF_H
#define __USB_CONF_H

#include <stdint.h>

/*******************************************************************************
*                              FIFO Size Configuration
*  
*  (i) Dedicated data FIFO SPRAM of 1.25 Kbytes = 1280 bytes = 320 32-bits words
*      available for the endpoints IN and OUT.
*      Device mode features:
*      -1 bidirectional CTRL EP 0
*      -3 IN EPs to support any kind of Bulk, Interrupt or Isochronous transfer
*      -3 OUT EPs to support any kind of Bulk, Interrupt or Isochronous transfer
*
*  ii) Receive data FIFO size = RAM for setup packets + 
*                   OUT endpoint control information +
*                   data OUT packets + miscellaneous
*      Space = ONE 32-bits words
*     --> RAM for setup packets = 4 * n + 6 space
*        (n is the nbr of CTRL EPs the device core supports) 
*     --> OUT EP CTRL info      = 1 space
*        (one space for status information written to the FIFO along with each 
*        received packet)
*     --> data OUT packets      = (Largest Packet Size / 4) + 1 spaces 
*        (MINIMUM to receive packets)
*     --> OR data OUT packets  = at least 2*(Largest Packet Size / 4) + 1 spaces 
*        (if high-bandwidth EP is enabled or multiple isochronous EPs)
*     --> miscellaneous = 1 space per OUT EP
*        (one space for transfer complete status information also pushed to the 
*        FIFO with each endpoint's last packet)
*
*  (iii)MINIMUM RAM space required for each IN EP Tx FIFO = MAX packet size for 
*       that particular IN EP. More space allocated in the IN EP Tx FIFO results
*       in a better performance on the USB and can hide latencies on the AHB.
*
*  (iv) TXn min size = 16 words. (n  : Transmit FIFO index)
*   (v) When a TxFIFO is not used, the Configuration should be as follows: 
*       case 1 :  n > m    and Txn is not used    (n,m  : Transmit FIFO indexes)
*       --> Txm can use the space allocated for Txn.
*       case2  :  n < m    and Txn is not used    (n,m  : Transmit FIFO indexes)
*       --> Txn should be configured with the minimum space of 16 words
*  (vi) The FIFO is used optimally when used TxFIFOs are allocated in the top 
*       of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
*******************************************************************************/

const uint32_t RX_FIFO_SIZE = 128;
const uint32_t TX0_FIFO_SIZE = 64;
const uint32_t TX1_FIFO_SIZE = 64;
const uint32_t TX2_FIFO_SIZE = 16;
const uint32_t TX3_FIFO_SIZE = 16;

/* OTGD-FS-DEVICE IP interrupts Enable definitions */
/* Uncomment the define to enable the selected interrupt */
const uint8_t INTR_MODEMISMATCH=0;
const uint8_t INTR_SOFINTR=0;
const uint8_t INTR_RXSTSQLVL=1;           /* Mandatory */
const uint8_t INTR_NPTXFEMPTY=0;
const uint8_t INTR_GINNAKEFF=0;
const uint8_t INTR_GOUTNAKEFF=0;
const uint8_t INTR_ERLYSUSPEND=0;
const uint8_t INTR_USBSUSPEND=1;          /* Mandatory */
const uint8_t INTR_USBRESET=1;            /* Mandatory */
const uint8_t INTR_ENUMDONE=1;         /* Mandatory */
const uint8_t INTR_ISOOUTDROP=0;
const uint8_t INTR_EOPFRAME=0;
const uint8_t INTR_EPMISMATCH=0;
const uint8_t INTR_INEPINTR=1;            /* Mandatory */
const uint8_t INTR_OUTEPINTR=1;           /* Mandatory */
const uint8_t INTR_INCOMPLISOIN=0;
const uint8_t INTR_INCOMPLISOOUT=0;
const uint8_t INTR_WKUPINTR=1;            /* Mandatory */
const uint8_t INTR_DISCONNECT=0;

#endif /* __USB_CONF_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
