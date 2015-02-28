/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_istr.c
* Author             : MCD Application Team
* Version            : V3.1.1
* Date               : 04/07/2010
* Description        : ISTR events interrupt service routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "stm32_usb_pwr.h"
#include "stm32_usb_istr.h"
#include "stm32_usb_core.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t wIstr;  /* ISTR register last read value */
__IO uint8_t bIntPackSOF = 0;  /* SOFs received between 2 consecutive packets */

/* Extern variables ----------------------------------------------------------*/
/** enabled interrupts
  */
extern const uint8_t INTR_MODEMISMATCH;
extern const uint8_t INTR_SOFINTR;
extern const uint8_t INTR_RXSTSQLVL;
extern const uint8_t INTR_NPTXFEMPTY;
extern const uint8_t INTR_GINNAKEFF;
extern const uint8_t INTR_GOUTNAKEFF;
extern const uint8_t INTR_ERLYSUSPEND;
extern const uint8_t INTR_USBSUSPEND;
extern const uint8_t INTR_USBRESET;
extern const uint8_t INTR_ENUMDONE;
extern const uint8_t INTR_ISOOUTDROP;
extern const uint8_t INTR_EOPFRAME;
extern const uint8_t INTR_EPMISMATCH;
extern const uint8_t INTR_INEPINTR;
extern const uint8_t INTR_OUTEPINTR;
extern const uint8_t INTR_INCOMPLISOIN;
extern const uint8_t INTR_INCOMPLISOOUT;
extern const uint8_t INTR_DISCONNECT;
extern const uint8_t INTR_WKUPINTR;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#pragma weak EP1_IN_Callback 
#pragma weak EP2_IN_Callback 
#pragma weak EP3_IN_Callback 
#pragma weak EP4_IN_Callback 
#pragma weak EP5_IN_Callback 
#pragma weak EP6_IN_Callback 
#pragma weak EP7_IN_Callback 
#pragma weak EP1_OUT_Callback 
#pragma weak EP2_OUT_Callback 
#pragma weak EP3_OUT_Callback 
#pragma weak EP4_OUT_Callback 
#pragma weak EP5_OUT_Callback 
#pragma weak EP6_OUT_Callback 
#pragma weak EP7_OUT_Callback 
/* function pointers to non-control endpoints service routines */
void (*pEpInt_IN[7])(void) =
  {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
    EP5_IN_Callback,
    EP6_IN_Callback,
    EP7_IN_Callback,
  };

void (*pEpInt_OUT[7])(void) =
  {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
    EP5_OUT_Callback,
    EP6_OUT_Callback,
    EP7_OUT_Callback,
  };

/*******************************************************************************
* Function Name  : STM32_PCD_OTG_ISR_Handler
* Description    : Handles all USB Device Interrupts
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
unsigned int STM32_PCD_OTG_ISR_Handler (void)
{
  USB_OTG_int_sts_data gintr_status;
  unsigned int retval = 0;

  if (IsDeviceMode()) /* ensure that we are in device mode */
  {
    gintr_status.d32 = OTGD_FS_ReadCoreItr();

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    
    /* If there is no interrupt pending exit the interrupt routine */
    if (!gintr_status.d32)
    {
      return 0;
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Early Suspend interrupt */ 
    if( INTR_ERLYSUSPEND ) if (gintr_status.b.erlysuspend)
    {
      retval |= OTGD_FS_Handle_EarlySuspend_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* End of Periodic Frame interrupt */
    if( INTR_EOPFRAME ) if (gintr_status.b.eopframe)
    {
      retval |= OTGD_FS_Handle_EOPF_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Non Periodic Tx FIFO Emty interrupt */
    if( INTR_NPTXFEMPTY ) if (gintr_status.b.nptxfempty)
    {
      retval |= OTGD_FS_Handle_NPTxFE_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Wakeup or RemoteWakeup interrupt */
    if( INTR_WKUPINTR ) if (gintr_status.b.wkupintr)
    {   
      retval |= OTGD_FS_Handle_Wakeup_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Suspend interrupt */
    if( INTR_USBSUSPEND ) if (gintr_status.b.usbsuspend)
    { 
      /* check if SUSPEND is possible */
      if (fSuspendEnabled)
      {
        Suspend();
      }
      else
      {
        /* if not possible then resume after xx ms */
        Resume(RESUME_LATER); /* This case shouldn't happen in OTG Device mode because 
        there's no ESOF interrupt to increment the ResumeS.bESOFcnt in the Resume state machine */
      }
            
      retval |= OTGD_FS_Handle_USBSuspend_ISR();
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Start of Frame interrupt */
    if( INTR_SOFINTR ) if (gintr_status.b.sofintr)
    {
      /* Update the frame number variable */
      bIntPackSOF++;
      
      retval |= OTGD_FS_Handle_Sof_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Receive FIFO Queue Status Level interrupt */
    if( INTR_RXSTSQLVL ) if (gintr_status.b.rxstsqlvl)
    {
      retval |= OTGD_FS_Handle_RxStatusQueueLevel_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Enumeration Done interrupt */
    if( INTR_ENUMDONE ) if (gintr_status.b.enumdone)
    {
      retval |= OTGD_FS_Handle_EnumDone_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Reset interrutp */
    if( INTR_USBRESET ) if (gintr_status.b.usbreset)
    {
      retval |= OTGD_FS_Handle_UsbReset_ISR();
    }    
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* IN Endpoint interrupt */
    if( INTR_INEPINTR ) if (gintr_status.b.inepint)
    {
      retval |= OTGD_FS_Handle_InEP_ISR();
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* OUT Endpoint interrupt */
    if( INTR_OUTEPINTR ) if (gintr_status.b.outepintr)
    {
      retval |= OTGD_FS_Handle_OutEP_ISR();
    }
 
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Mode Mismatch interrupt */
    if( INTR_MODEMISMATCH ) if (gintr_status.b.modemismatch)
    {
      retval |= OTGD_FS_Handle_ModeMismatch_ISR();
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Global IN Endpoints NAK Effective interrupt */
    if( INTR_GINNAKEFF ) if (gintr_status.b.ginnakeff)
    {
      retval |= OTGD_FS_Handle_GInNakEff_ISR();
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Global OUT Endpoints NAK effective interrupt */
    if( INTR_GOUTNAKEFF ) if (gintr_status.b.goutnakeff)
    {
      retval |= OTGD_FS_Handle_GOutNakEff_ISR();
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Isochrounous Out packet Dropped interrupt */
    if( INTR_ISOOUTDROP ) if (gintr_status.b.isooutdrop)
    {
      retval |= OTGD_FS_Handle_IsoOutDrop_ISR();
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Endpoint Mismatch error interrupt */
    if( INTR_EPMISMATCH ) if (gintr_status.b.epmismatch)
    {
      retval |= OTGD_FS_Handle_EPMismatch_ISR();
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Incomplete Isochrous IN tranfer error interrupt */
    if( INTR_INCOMPLISOIN ) if (gintr_status.b.incomplisoin)
    {
      retval |= OTGD_FS_Handle_IncomplIsoIn_ISR();
    }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Incomplete Isochrous OUT tranfer error interrupt */
    if( INTR_INCOMPLISOOUT ) if (gintr_status.b.outepintr)
    {
      retval |= OTGD_FS_Handle_IncomplIsoOut_ISR();
    }
  
  }
  return retval;
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
