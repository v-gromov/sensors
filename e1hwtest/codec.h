#ifndef CODEC__H
#define CODEC__H

#define TLV320AIC_ADDR 0xE2

int codec_rw( void );
void codec_init( void );
int codec_write( int addr, int data );
int codec_read( int addr );

#endif //CODEC__H
