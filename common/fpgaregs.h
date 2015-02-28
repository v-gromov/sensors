#ifndef C_FPGAREGS__H
#define C_FPGAREGS__H

#include <stdint.h>

/** fpga register access operation codes
 */
#define FPGA_COP_WR  0x0
#define FPGA_COP_RD  0x1

int fpgaregs_read( int address );
int fpgaregs_write( int address, int data );
void fpga_rise_bit( uint16_t addr, uint16_t bit );
void fpga_fall_bit( uint16_t addr, uint16_t bit );
void fpga_set_bit( uint16_t addr, uint16_t bit );
void fpga_unset_bit( uint16_t addr, uint16_t bit );
unsigned char fpga_check_bit( uint16_t addr, uint16_t bit );
void fpga_check_cond_and_set_bit( uint16_t addr, uint16_t bit,
                                  unsigned char cond );
void fpga_set_mask_val8( uint16_t addr, uint8_t offs, uint8_t mask, 
                          uint8_t val );
uint8_t fpga_get_mask_val8( uint16_t addr, uint8_t offs, uint8_t mask );
void fpga_set_mask_val16( uint16_t addr, uint16_t offs, uint16_t mask,
                          uint16_t val );
void fpga_set_mask_val32( uint16_t addr, uint16_t offs, uint32_t mask,
                          uint32_t val );
#endif
