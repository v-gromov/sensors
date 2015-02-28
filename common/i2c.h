#ifndef I2C__H
#define I2C__H

#include <stdint.h>

/** \addtogroup I2C 
  * \{
  */

/*@{*/

#define FLAG_Mask               ((uint32_t)0x00FFFFFF)
#define CR1_CLEAR_Mask          ((uint16_t)0xFBF5)      /* I2C registers Masks */
#define CCR_CCR_Set             ((uint16_t)0x0FFF)      /* I2C CCR mask */
#define CCR_FS_Set              ((uint16_t)0x8000)      /* I2C F/S mask */

#define ENABLE 1
#define DISABLE 0
#define ERROR 0
#define SUCCESS 1

/** 
  * @brief  EV5
  */
#define  I2C_EVENT_MASTER_MODE_SELECT                      ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */

/** 
  * @brief  EV8_2
  */
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */

/** 
  * @brief  EV6
  */
#define  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED        ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED           ((uint32_t)0x00030002)  /* BUSY, MSL and ADDR flags */

/* SR2 flags */
#define I2C_FLAG_BUSY                   ((uint32_t)0x00020000)
/* SR1 flags */
#define I2C_FLAG_ADDR                   ((uint32_t)0x10000002)

int TwInit( uint8_t sla );
int TwMasterRegWrite( uint8_t sla, uint32_t iadr, uint8_t iadrlen, const void *txdata, uint16_t txsiz );
int TwMasterRegRead( uint8_t sla, uint32_t iadr, uint8_t iadrlen, void *rxdata, uint16_t rxsiz );
int TwMasterTransact( uint8_t sla, const void *txdata, uint16_t txlen,
                      void *rxdata, uint16_t rxsiz );

/*@}*/
#endif //I2C__H
