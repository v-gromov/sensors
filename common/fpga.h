#ifndef FPGA__H
#define FPGA__H

/** \addtogroup FPGA_CONF
  * \{
  */

/*@{*/

#define EBADBLOCK    1
void fpga_reset(void);
int fpga_begin_conf(void);
int fpga_confdone( void );
int fpga_send_ps(unsigned char* block,int count);

/*@}*/
#endif // FPGA__H
