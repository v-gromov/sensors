#ifndef DATAFLASH__H
#define DATAFLASH__H

/** \addtogroup DataFlash
  * \{
  */

/*@{*/
#define DF_BLOCK_SIZE 4096
#define DF_PAGE_SIZE  256

void dataflash_init();
void dataflash_block_read( unsigned long address, unsigned char * buffer,
                      unsigned short size );
void dataflash_block_write( unsigned long address, unsigned char * buffer,
                      unsigned short size );
void dataflash_chip_erase( );
void dataflash_block_4k_erase( unsigned long address );
void dataflash_block_64k_erase( unsigned long address );
void dataflash_global_unlock( );
void dataflash_we( unsigned char state );

/*@}*/
#endif // DATAFLASH__H
