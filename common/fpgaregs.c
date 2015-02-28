#include <common/fpgaregs.h>
#include <common/spi.h>

/** \brief Perform read from FPGA 
  *
  * Read the register addressed by the 'address'.
  * Address is 12 bit wide.
  * Access to fpga is made in LSB manner.
  * \param address -- register address
  *
  * \return register value.
 */
int fpgaregs_read( int address )
{
  uint8_t buf[5];
  buf[0] = FPGA_COP_RD | ( ( address & 0xfUL ) << 4 );
  buf[1] = ( address >> 4 ) & 0xff;
  buf[2] = buf[3] = buf[4] = 0xff;      // dummy bytes

  spi1_transfer( 5, buf, buf );

  return buf[3] | ( ( ( unsigned short ) buf[4] ) << 8 );
};

/** \brief Perform write to FPGA
  * 
  * Write data to the register addressed by the 'address'.
  * Address is 12 bit wide.
  * Access to fpga is made in LSB manner.
  * \param address -- register address
  * \param data -- data to write to the register
  *
  * \return written value
 */
int fpgaregs_write( int address, int data )
{
  uint8_t buf[5];
  buf[0] = FPGA_COP_WR | ( ( address & 0xfUL ) << 4 );
  buf[1] = ( address >> 4 ) & 0xff;
  buf[2] = 0xff;                // dummy bytes
  buf[3] = data & 0xff;         // low byte
  buf[4] = ( data >> 8 ) & 0xff;        // high byte

  spi1_transfer( 5, buf, 0 );

  return data;
};

/** 
 * \brief Set bit to 0, then rise it to 1
 * 
 * \param addr FPGA register
 * \param bit Target bit
 */
void fpga_rise_bit( uint16_t addr, uint16_t bit ) 
{
  uint16_t reg = fpgaregs_read( addr );
  fpgaregs_write( addr, reg & ~bit );
  fpgaregs_write( addr, reg | bit );
}; 

/** 
 * \brief Set bit to 1, then fall it to 0
 * 
 * \param addr FPGA register
 * \param bit Target bit
 */
void fpga_fall_bit( uint16_t addr, uint16_t bit ) 
{
  uint16_t reg = fpgaregs_read( addr );
  fpgaregs_write( addr, reg | bit );
  fpgaregs_write( addr, reg & ~bit );
} 

/** 
 * \brief Set bit in FPGA register
 * 
 * \param addr FPGA register
 * \param bit Bit
 */
void fpga_set_bit( uint16_t addr, uint16_t bit ) 
{
  uint16_t reg = fpgaregs_read( addr );
  reg |= bit;
  fpgaregs_write( addr, reg );
} 

/** 
 * \brief Unset bit in FPGA register
 * 
 * \param addr FPGA register
 * \param bit Bit
 */
void fpga_unset_bit( uint16_t addr, uint16_t bit ) 
{
  uint16_t reg = fpgaregs_read( addr );
  reg &= ~bit;
  fpgaregs_write( addr, reg );
} 

/** 
 * \brief Checks specified bit in register with specified address 
 * 
 * \param addr - address of register to check a bit for
 * \param bit - number of a bit to check
 * 
 * \return 1 if a value of a bit is 1, 0 otherwise 
 */
unsigned char fpga_check_bit( uint16_t addr, uint16_t bit )
{
  uint16_t reg = fpgaregs_read( addr );
  return ( reg & bit ) != 0;
}

/** \brief Sets bit if cond is true and clear bit if cond is false
 *  \param addr - address of register to set or unset a bit for
 *  \param bit - number of a bit to set or to unset
 *  \param cond - condition to check for further manipulation with
 *                specified bit
 */ 
void fpga_check_cond_and_set_bit( uint16_t addr, uint16_t bit, 
    unsigned char cond ) 
{
  uint16_t reg = fpgaregs_read( addr );
  if (cond) {
    reg |= bit;
  } else {
    reg &= ~bit;
  }
  fpgaregs_write( addr, reg );
}

/** \brief Sets u8 masked value in register with offset
 *  \param addr addres of register
 *  \param offs value offset 
 *  \param mask value mask
 *  \param value wroted value
 */
void fpga_set_mask_val8( uint16_t addr, uint8_t offs, uint8_t mask, 
                          uint8_t val )
{
  uint8_t reg = fpgaregs_read( addr );
  reg &= ~mask;
  reg |= ( val << offs ) & mask; 
  fpgaregs_write( addr, reg );
}

/** \brief Gets u8 masked value in register with offset
 *  \param addr addres of register
 *  \param offs value offset 
 *  \param mask value mask
 *  \param value wroted value
 */
uint8_t fpga_get_mask_val8( uint16_t addr, uint8_t offs, uint8_t mask )
{
  uint8_t reg = fpgaregs_read( addr );

  return ( reg & mask ) >> offs;
}

/** \brief Sets u16 masked value in register with offset
 *  \param addr addres of register
 *  \param offs value offset 
 *  \param mask value mask
 *  \param value wroted value
 */
void fpga_set_mask_val16( uint16_t addr, uint16_t offs, uint16_t mask, 
                          uint16_t val )
{
  uint16_t reg = fpgaregs_read( addr );
  reg &= ~mask;
  reg |= ( val << offs ) & mask; 
  fpgaregs_write( addr, reg );
}

/** \brief Sets u32 masked value in two adjacent registers with offset
 *  \param addr addres of smaller register
 *  \param offs value offset 
 *  \param mask value mask
 *  \param value wroted value
 */
void fpga_set_mask_val32( uint16_t addr, uint16_t offs, uint32_t mask, 
                          uint32_t val )
{
  uint16_t l = fpgaregs_read( addr );
  uint16_t h = fpgaregs_read( addr + 1 );
  uint32_t r = l  | ( h << 16 );
  r &= ~( mask );
  r |= ( val << offs ) & mask;
  fpgaregs_write( addr,     ( uint16_t ) r );
  fpgaregs_write( addr + 1, ( uint16_t ) ( r >> 16 ) );
}
