#include <libopenstm32/i2c.h>
#include <libopenstm32/rcc.h>
#include <libopenstm32/gpio.h>
#include "i2c.h"

/** \file i2c.c
  * \brief This file contains functions for accessing I2C1 bus
  * \addtogroup i2c
  * \{
  */

/*@{*/

static void I2C_HardwareSetup_I2C1( void )
{
  //Remap I2C1
  rcc_peripheral_enable_clock( &RCC_APB2ENR, AFIOEN );
  AFIO_MAPR |= AFIO_MAPR_I2C1_REMAP;
  //Включить clock для I2C1
  rcc_peripheral_enable_clock( &RCC_APB1ENR, I2C1EN );
  gpio_set_mode( GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO8 | GPIO9 );
};

static void I2C_Recover( void )
{
  gpio_set_mode( GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_OPENDRAIN, GPIO8 | GPIO9 );

  //SDA -> high
  gpio_set( GPIOB, GPIO9 );
  if( !gpio_get( GPIOB, GPIO9 ) ) {
    while( !gpio_get( GPIOB, GPIO9 ) ) {
      gpio_clear( GPIOB, GPIO8 );
      gpio_set( GPIOB, GPIO8 );
    };
    gpio_set( GPIOB, GPIO9 );
    gpio_set( GPIOB, GPIO8 );
    gpio_clear( GPIOB, GPIO9 ); //SDA - start event
    gpio_set( GPIOB, GPIO9 );   //SDA - stop event
  };
};

static uint8_t I2C_ReceiveData( u32 I2Cx )
{
  /* Return the data in the DR register */
  return ( uint8_t ) I2C_DR( I2Cx );
}

uint32_t I2C_GetFlagStatus( u32 I2Cx, uint32_t I2C_FLAG )
{
  volatile uint32_t i2creg = 0, flag = 0;
  i2creg = I2C_FLAG >> 28;
  I2C_FLAG &= FLAG_Mask;
  if( i2creg != 0 ) {
    //SR1
    flag = I2C_SR1( I2Cx ) & I2C_FLAG;
  }
  else {
    //SR2
    I2C_FLAG = ( uint32_t ) ( I2C_FLAG >> 16 );
    flag = I2C_SR2( I2Cx ) & I2C_FLAG;
  };
  return ( flag ) ? 1 : 0;
};

static uint8_t I2C_CheckEvent( u32 I2Cx, uint32_t I2C_EVENT )
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  uint8_t status = ERROR;
  /* Read the I2Cx status register */
  flag1 = I2C_SR1( I2Cx );
  flag2 = I2C_SR2( I2Cx );
  flag2 = flag2 << 16;
  /* Get the last event value from I2C status register */
  lastevent = ( flag1 | flag2 ) & FLAG_Mask;
  /* Check whether the last event is equal to I2C_EVENT */
  if( lastevent == I2C_EVENT ) {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS;
  }
  else {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR;
  }
  /* Return status */
  return status;
}

static uint32_t I2C_GetLastEvent( u32 I2Cx )
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  /* Read the I2Cx status register */
  flag1 = I2C_SR1( I2Cx );
  flag2 = I2C_SR2( I2Cx );
  flag2 = flag2 << 16;
  /* Get the last event value from I2C status register */
  lastevent = ( flag1 | flag2 ) & FLAG_Mask;
  /* Return status */
  return lastevent;
}

static void I2C_AcknowledgeConfig( u32 I2Cx, uint8_t NewState )
{
  if( NewState != DISABLE ) {
    /* Enable the acknowledgement */
    I2C_CR1( I2Cx ) |= I2C_CR1_ACK;
  }
  else {
    /* Disable the acknowledgement */
    I2C_CR1( I2Cx ) &= ~( I2C_CR1_ACK );
  }
}

static void I2C_Send7bitAddress( u32 I2Cx, uint8_t Address,
                          uint8_t I2C_Direction )
{
  /* Test on the direction to set/reset the read/write bit */
  if( I2C_Direction != I2C_WRITE ) {
    /* Set the address bit0 for read */
    Address |= I2C_READ;
  }
  else {
    /* Reset the address bit0 for write */
    Address &= ~( I2C_READ );
  }
  /* Send the address */
  I2C_DR( I2Cx ) = Address;
}

static void I2C_Master_BufferWrite( uint8_t * pBuffer,
                             uint16_t NumByteToWrite )
{
  i2c_send_data( I2C1, *pBuffer );
  pBuffer++;
  NumByteToWrite--;
  /* While there is data to be written */
  while( NumByteToWrite-- ) {
    while( ( I2C_GetLastEvent( I2C1 ) & 0x04 ) != 0x04 )
      continue;                 //Poll on BTF
    /* Send the current byte */
    i2c_send_data( I2C1, *pBuffer );
    /* Point to the next byte to be written */
    pBuffer++;

  }
  /* Test on EV8_2 and clear it, BTF = TxE = 1, DR and shift registers are
     empty */
  while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
    continue;
}

static void I2C_Master_BufferRead( uint8_t * pBuffer,
                            uint16_t NumByteToRead )
{
  while( !I2C_CheckEvent
         ( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) )
    continue;
  /* While there is data to be read; here the safe procedure is implemented */
  while( NumByteToRead ) {
    if( NumByteToRead != 3 ) {  /* Receive bytes from first byte until byte N-3 */
      while( ( I2C_GetLastEvent( I2C1 ) & 0x00004 ) != 0x000004 )
        continue;               /* Poll on BTF */
      /* Read data */
      *pBuffer = I2C_ReceiveData( I2C1 );
      pBuffer++;
      /* Decrement the read bytes counter */
      NumByteToRead--;
    }
    if( NumByteToRead == 3 ) {  /* it remains to read three data: data N-2, data N-1, Data N */
      /* Data N-2 in DR and data N -1 in shift register */
      while( ( I2C_GetLastEvent( I2C1 ) & 0x000004 ) != 0x0000004 )
        continue;               /* Poll on BTF */
      /* Clear ACK */
      I2C_AcknowledgeConfig( I2C1, DISABLE );
      /* Read Data N-2 */
      *pBuffer = I2C_ReceiveData( I2C1 );
      pBuffer++;
      /* Program the STOP */
      i2c_send_stop( I2C1 );
      /* Read DataN-1 */
      *pBuffer = I2C_ReceiveData( I2C1 );
      pBuffer++;
      while( ( I2C_GetLastEvent( I2C1 ) & 0x00000040 ) != 0x0000040 )
        continue;               /* Poll on RxNE */
      /* Read DataN */
      *pBuffer = I2C_DR( I2C1 );
      /* Reset the number of bytes to be read by master */
      NumByteToRead = 0;
    }
  }
  /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
  while( ( I2C_CR1( I2C1 ) & 0x200 ) == 0x200 )
    continue;
  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig( I2C1, ENABLE );
}

static uint8_t I2C_Master_BufferRead1Byte( void )
{
  uint8_t Data;
  volatile uint32_t temp;
  /* Wait until ADDR is set */
  while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_ADDR ) )
    continue;
  /* Clear ACK */
  I2C_AcknowledgeConfig( I2C1, DISABLE );
  /* Clear ADDR flag */
  temp = I2C_SR2( I2C1 );
  /* Program the STOP */
  i2c_send_stop( I2C1 );
  while( ( I2C_GetLastEvent( I2C1 ) & 0x0040 ) != 0x000040 )
    continue;                   /* Poll on RxNE */
  /* Read the data */
  Data = I2C_ReceiveData( I2C1 );
  /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
  while( ( I2C_CR1( I2C1 ) & 0x200 ) == 0x200 )
    continue;
  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig( I2C1, ENABLE );
  return ( Data );

}

static void I2C_Master_BufferRead2Byte( uint8_t * pBuffer )
{

  volatile uint32_t temp;
  /* Send EEPROM address for read */
  I2C_CR1( I2C1 ) = 0xC01;      /* ACK=1; POS =1 */
  while( !I2C_GetFlagStatus( I2C1, I2C_FLAG_ADDR ) )
    continue;
  /* Clear ADDR */
  temp = I2C_SR2( I2C1 );
  /* Disable ACK */
  I2C_AcknowledgeConfig( I2C1, DISABLE );
  while( ( I2C_GetLastEvent( I2C1 ) & 0x0004 ) != 0x00004 )
    continue;                   /* Poll on BTF */
  /* Program the STOP */
  i2c_send_stop( I2C1 );
  /* Read first data */
  *pBuffer = I2C_DR( I2C1 );
  pBuffer++;
  /* Read second data */
  *pBuffer = I2C_DR( I2C1 );
  I2C_CR1( I2C1 ) = 0x0401;     /* POS = 0, ACK = 1, PE = 1 */
}

void I2C_SendStartAndWait( void )
{
  /* Send START condition */
  i2c_send_start( I2C1 );
  /* Test on EV5 and clear it */
  while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) )
    continue;
};

/*!
 * \brief Transmit and/or receive data as a master.
 *
 * The two-wire serial interface must have been initialized by calling
 * TwInit() before this function can be used.
 *
 * \param sla    Slave address of the destination. This slave address
 *               must be specified as a 7-bit address. For example, the
 *               PCF8574A may be configured to slave addresses from 0x38
 *               to 0x3F.
 * \param txdata Points to the data to transmit. Ignored, if the number
 *               of data bytes to transmit is zero.
 * \param txlen  Number of data bytes to transmit. If zero, then the
 *               interface will not send any data to the slave device
 *               and will directly enter the master receive mode.
 * \param rxdata Points to a buffer, where the received data will be
 *               stored. Ignored, if the maximum number of bytes to
 *               receive is zero.
 * \param rxsiz  Maximum number of bytes to receive. Set to zero, if
 *               no bytes are expected from the slave device.
 *
 * \return The number of bytes received, -1 in case of an error or timeout.
 */
int TwMasterTransact( uint8_t sla, const void *txdata, uint16_t txlen,
                      void *rxdata, uint16_t rxsiz )
{
  if( txlen ) {
    //Transmit data
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) )
      continue;
    I2C_SendStartAndWait(  );
    I2C_Send7bitAddress( I2C1, sla, I2C_WRITE );
    /* Test on EV6 and clear it */
    while( !I2C_CheckEvent
           ( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )
      continue;
    I2C_Master_BufferWrite( ( uint8_t * ) txdata, txlen );
    i2c_send_stop( I2C1 );
  };
  if( rxsiz ) {
    //Receive data
    I2C_SendStartAndWait(  );
    I2C_Send7bitAddress( I2C1, sla, I2C_READ );
    switch ( rxsiz ) {
      case 1:
        ( ( uint8_t * ) rxdata )[0] = I2C_Master_BufferRead1Byte(  );
        break;
      case 2:
        I2C_Master_BufferRead2Byte( rxdata );
        break;
      default:
        I2C_Master_BufferRead( rxdata, rxsiz );
        break;
    };
  };

  return 0;
}

/*!
 * \brief Receive data as a master from a device having internal addressable registers
 *
 * The two-wire serial interface must have been initialized by calling
 * TwInit() before this function can be used.
 *
 * \param sla     Slave address of the destination. This slave address
 *                must be specified as a 7-bit address. For example, the
 *                PCF8574A may be configured to slave addresses from 0x38
 *                to 0x3F.
 * \param iadr    Address send to the device to access certain registers
 *                or memory addresses of it.
 * \param iadrlen Number of bytes to send as address, maximum 3 bytes are
 *                supported from AT91SAM7
 * \param rxdata  Points to a buffer, where the received data will be
 *                stored.
 * \param rxsiz   Maximum number of bytes to receive.
 *
 * \return The number of bytes received, -1 in case of an error or timeout.
 */
int TwMasterRegRead( uint8_t sla, uint32_t iadr, uint8_t iadrlen, void *rxdata, uint16_t rxsiz )        
{
  if( rxsiz == 0 ) {
    return -1;
  }

  //Transmit addr
  while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) )
    continue;
  I2C_SendStartAndWait(  );
  I2C_Send7bitAddress( I2C1, sla, I2C_WRITE );
  /* Test on EV6 and clear it */
  while( !I2C_CheckEvent
         ( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )
    continue;
  I2C_Master_BufferWrite( ( uint8_t * ) & iadr, iadrlen );      
  if( rxsiz ) {
    I2C_SendStartAndWait(  );
    I2C_Send7bitAddress( I2C1, sla, I2C_READ );
    switch ( rxsiz ) {
      case 1:
        ( ( uint8_t * ) rxdata )[0] = I2C_Master_BufferRead1Byte(  );
        break;
      case 2:
        I2C_Master_BufferRead2Byte( rxdata );
        break;
      default:
        I2C_Master_BufferRead( rxdata, rxsiz );
        break;
    };
  }
  else {
    i2c_send_stop( I2C1 );
  };

  return 0;
}

/*!
 * \brief Transmit data as a master to a device having internal addressable registers
 *
 * The two-wire serial interface must have been initialized by calling
 * TwInit() before this function can be used.
 *
 * \param sla     Slave address of the destination. This slave address
 *                must be specified as a 7-bit address. For example, the
 *                PCF8574A may be configured to slave addresses from 0x38
 *                to 0x3F.
 * \param iadr    Address send to the device to access certain registers
 *                or memory addresses of it.
 * \param iadrlen Number of bytes to send as address, maximum 3 bytes are
 *                supported from AT91SAM7
 * \param txdata  Points to a buffer, where the data to transmit will be
 *                stored.
 * \param txsiz   Maximum number of bytes to transmit.
 *
 * \return        The number of bytes transmitted, -1 in case of an error
 *                or timeout. Number could be less if slave end transmission
 *                with NAK.
 */

int TwMasterRegWrite( uint8_t sla, uint32_t iadr, uint8_t iadrlen, const void *txdata, uint16_t txsiz ) 
{
  while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) )
    continue;
  I2C_SendStartAndWait(  );
  I2C_Send7bitAddress( I2C1, sla, I2C_WRITE );
  /* Test on EV6 and clear it */
  while( !I2C_CheckEvent
         ( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )
    continue;
  I2C_Master_BufferWrite( ( uint8_t * ) & iadr, iadrlen );      
  if( txsiz ) {
    I2C_Master_BufferWrite( ( uint8_t * ) txdata, txsiz );
  };
  i2c_send_stop( I2C1 );

  return 0;
}

static int TwSetSpeed( uint32_t speed )
{
  uint16_t result = 0;
  i2c_peripheral_disable( I2C1 );
  /* Reset tmpreg value */
  /* Configure speed in standard mode */
  /* Standard mode speed calculate */
  result = ( uint16_t ) ( 36000000 / ( speed << 1 ) );
  /* Test if CCR value is under 0x4 */
  if( result < 0x04 ) {
    /* Set minimum allowed value */
    result = 0x04;
  }
  /* Set speed value for standard mode */
  /* Set Maximum Rise Time for standard mode */
  i2c_set_trise( I2C1, 71 );
  /* Write to I2C CCR */
  I2C_CCR( I2C1 ) = result;
  /* Enable the selected I2C peripheral */
  i2c_peripheral_enable( I2C1 );

  return 0;
}

/*!
 * \brief Initialize TWI interface.
 *
 * \param sla Slave address, must be specified as a 7-bit address,
 *            always lower than 128.
 */
int TwInit( uint8_t sla )
{
  uint32_t speed = 100000;
  uint16_t tmpreg = 0;

  I2C_Recover(  );
  I2C_HardwareSetup_I2C1(  );

  i2c_peripheral_disable( I2C1 );
  I2C_CR1( I2C1 ) |= I2C_CR1_SWRST;     //reset
  I2C_CR1( I2C1 ) &= ~I2C_CR1_SWRST;    //end_of reset

  //Задать clock для I2C1 контроллераa
  i2c_set_clock_frequency( I2C1, I2C_CR2_FREQ_36MHZ );

  /* Set initial rate. */
  TwSetSpeed( speed );

  //Clear flags.
  tmpreg = I2C_CR1( I2C1 );
  tmpreg &= CR1_CLEAR_Mask;
  tmpreg |= I2C_CR1_ACK;
  I2C_CR1( I2C1 ) = tmpreg;
  //
  i2c_set_own_7bit_slave_address( I2C1, sla );

  return 0;
}

/*@}*/
