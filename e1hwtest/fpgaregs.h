#ifndef FPGAREGS__H
#define FPGAREGS__H

#include <common/defs.h>

/*
 * fpga registers and bits
 */
#define FPGA_REG_WIDTH  (16)    // width of fpga's registers in bits
#define FPGA_REG_MASK ((1<<FPGA_REG_WIDTH) - 1)

#define CTRL_OFFSET   0x000
#define STAT_OFFSET   0x040
#define FPGA_REGS_COUNT 0x080   // how many registers in fpga - max limit

#define CR(o)            (CTRL_OFFSET + (o))
#define SR(o)            (STAT_OFFSET + (o))

#define FPGA_SECR        CR(0)  // subsystem enable control register

// pcm control, adc control
#define FPGA_RPCR        CR(0)  // rx pcm control register
#define FPGA_TPCR        CR(1)  // tx pcm control register
#define FPGA_RPECSR      CR(1)  // rx pcm error counters select register - 4 bits
#define FPGA_ADCCR       CR(1)  // adc control register
#define FPGA_RPDVCR      CR(2)  // rx pcm data view control register

// rx pcm bertest, tx pcm bertest
#define FPGA_PRBSCR      CR(2)  // common for rx&tx bertest configuration
#define FPGA_RPBTCR      CR(2)  // rx pcm bertest control register

#define FPGA_RPBTTR0     CR(3)  // rx bert timeslot mask - ts from 0 to 15
#define FPGA_RPBTTR1     CR(4)  // rx bert timeslot mask - ts from 16 to 31
#define FPGA_TPPTR0      CR(5)  // tx prbs timeslot mask - ts from 0 to 15
#define FPGA_TPPTR1      CR(6)  // tx prbs timeslot mask - ts from 16 to 31

#define FPGA_UPATR0      CR(7)  // user pattern : bits from 0 to 15
#define FPGA_UPATR1      CR(8)  // user pattern : bits from 16 to 23
#define FPGA_TPPCR       CR(8)  // tx pcm prbs control register
#define FPGA_ELFIFO      CR(8)  // control elast fifo

// tx pcm bertest
#define FPGA_TPEGR       CR(9)  // tx pcm error generator control register
#define FPGA_LICR        CR(9)  // line interface control register
#define FPGA_CVMSKD      CR(9)  // маска для разрешения подсчета кодовых ошибок
#define FPGA_TPSIL       CR(10) // tx pcm silence byte
#define FPGA_CVMSKG      CR(10) // маска типов генерируемых кодовых ошибок

// tx pcm Si, Sa bits
#define FPGA_TPSISA4R    CR(11) // (Si[7..0], Sa4[7..0])
#define FPGA_TPSA65R     CR(12) // (Sa6[7..0], Sa5[7..0])
#define FPGA_TPSA87R     CR(13) // (Sa8[7..0], Sa7[7..0])

// tx pcm CAS registers
//  high byte      low byte
// (ch1  / ch16, 0000 / xyxx) = (ts1, ts0)
// (ch3  / ch18, ch2  / ch17) = (ts3, ts2)
// (ch5  / ch20, ch4  / ch19) = (ts5, ts4)
// (ch7  / ch22, ch6  / ch21) = (ts7, ts6)
// (ch9  / ch24, ch8  / ch23) = (ts9, ts8)
// (ch11 / ch26, ch10 / ch25) = (ts11, ts10)
// (ch13 / ch28, ch12 / ch27) = (ts13, ts12)
// (ch15 / ch30, ch14 / ch29) = (ts15, ts14)
#define FPGA_TPCASR(i)   CR(14 + i)

// tx pcm errors & alarms insertion
#define FPGA_TPEGPR0     CR(22) // tx errgen period register #0
#define FPGA_TPEGPR1     CR(23) // tx errgen period register #1, bits 16..23
#define FPGA_PCRTS       CR(23) // v/f rx timeslot (high byte of TPEGPR1)
#define FPGA_TPEGCR      CR(24) // tx errgen count register

// numerically controlled oscillator phase value
#define FPGA_NCOPH0      CR(25) // NCO phase: bits 15..0
#define FPGA_NCOPH1      CR(26) // bits 23..16 (low byte)
#define FPGA_JGPHR0      CR(26) // jitter gen phase value: bits 7..0 (high byte)
#define FPGA_JGPHR1      CR(27) // bits 23..8
#define FPGA_PCTTMR0     CR(28) // v/f tx ts mask: ts from 1 to 15
#define FPGA_PCTTMR1     CR(29) // ts from 16 to 31

#define FPGA_COPYCR      CR(30) // register for copying bits
#define FPGA_JGEND       CR(30) // jitter generator data register
#define FPGA_JGENCR      CR(31) // jitter generator control register

#define FPGA_ADURCR      CR(32) // alarm duration (1bit=1ms)

#define FPGA_KEYCR_LO    CR(33) //
#define FPGA_KEYCR_HI    CR(34) //

#define FPGA_XXX         CR(35) //nc
#define FPGA_SNCR        CR(35) //serial number

#define FPGA_VFRXTS      CR(36) //voice rx timeslot's number
#define FPGA_ADCCR2	 CR(37) //selects adc port to read

#define FPGA_CTRL_CNT    38

/*
 * Status registers
 */
#define FPGA_VSR         SR(0)  // version register
#define FPGA_FEAT        SR(1)  // fw features
#define FPGA_SR1         SR(1)  //
#define FPGA_RPECR       SR(2)  // rx pcm error counter
#define FPGA_RPAR        SR(3)  // rx pcm alarms register
#define FPGA_RPAER       SR(4)  // rx pcm alarms bit-wise enable register
#define FPGA_OSCSR       SR(5)  // oscilliscope data register
#define FPGA_SSCSR       SR(6)  // stroboscope data register
#define FPGA_SLSR        SR(7)  // signal level from peak detector
#define FPGA_JHP1SR      SR(8)  // jitter measured with HP1
#define FPGA_JHP2SR      SR(9)  // jitter measured with HP2
#define FPGA_FREQR0      SR(10) // measured frequency
#define FPGA_FREQR1      SR(11)
#define FPGA_VFRMSR      SR(12) // Voice RMS
#define FPGA_VFFR        SR(13) // Voice frequency
#define FPGA_RTD0        SR(14) // Measured Round Trip Delay (RTD)
#define FPGA_RTD1        SR(15)
#define FPGA_JGD0        SR(16) // регистры 0,1 для чтения значения
#define FPGA_JGD1        SR(17) // синусоиды из памяти генератора джиттера.
#define FPGA_FRMDVR      SR(18)
#define FPGA_OPTSR       SR(19) //available options
#define FPGA_INP_FIFO_LVL SR(20)        //
#define FPGA_TIE_LO      SR(22) // current TIE value
#define FPGA_TIE_HI      SR(23) // current TIE value
#define FPGA_STAT_CNT    (25)

/*
 * Control registers' bits
 */

// SECR/RPCR
#define FPGA_SECR_WAND  __BV(0) // enable/disable transmit wander
#define FPGA_SECR_TPEG   __BV(1)        // tx pcm error generator
#define FPGA_SECR_ADC    __BV(2)        // UNUSED
#define FPGA_SECR_PCMC   __BV(3)        // PCM-codec
#define FPGA_SECR_JITM   __BV(4)        // jitter measurements
#define FPGA_SECR_JITG   __BV(5)        // jitter generation
#define FPGA_SECR_SSC    __BV(6)        // stroboscope run bit, 0->1 runs, 1->0 stops
#define FPGA_SECR_OSC    __BV(7)        // oscilloscope run bit, 0->1 runs

#define FPGA_RPCR_UNSTR  __BV(8)        // unstructured flow enable
#define FPGA_RPCR_C4     __BV(9)        // crc4 checking
#define FPGA_RPCR_C4I    __BV(10)       // crc4 bits inversion
#define FPGA_RPCR_CAS    __BV(11)       // cas enable
#define FPGA_RPCR_MUXS   __BV(12)       // mux sync, switches receiver logics to sync port
#define FPGA_RPCR_FRRST  __BV(13)       // 0->1: reset frequency measurement
#define FPGA_RPCR_ALCPY  __BV(14)       // 0->1: copies alarms to buffer and clears them
#define FPGA_RPCR_CTP    __BV(15)       // code type 0 - AMI, 1 - HDB3

// TPCR/RPECSR/ADCCR
#define FPGA_TPCR_1KHZ   __BV(0)        // 1khz tone generation enable
#define FPGA_TPCR_TRINS  __BV(1)        // timeslot insertion into transit stream
#define FPGA_TPCR_VOEN   __BV(2)        // voice enable
#define FPGA_TPCR_TCSRC_OFF 3   // tclk source bits 1..0
#define FPGA_TPCR_TCSRC  BITMASK(2, FPGA_TPCR_TCSRC_OFF)
#define FPGA_TPCR_TREN   __BV(5)        // transit mode enable
#define FPGA_TPCR_RTDEN  __BV(6)        // Round Trip Delay measurements start

#define FPGA_RPECSR_SEL_OFF 7   // counter select bits
#define FPGA_RPECSR_SEL  BITMASK(4, FPGA_RPECSR_SEL_OFF)
#define FPGA_RPECSR_RST  __BV(11)       // transition 0->1 copies pcm counter to
                                // temporary buffer register

#define FPGA_ADCCR_GAIN_OFF 12  // adc gain
#define FPGA_ADCCR_GAIN  BITMASK(2, FPGA_ADCCR_GAIN_OFF)

// RPDVCR/PRBSCR/RPBTCR
#define FPGA_RPDVCR_TS_OFF  0   // data view timeslot bits 0..4
#define FPGA_RPDVCR_TS      BITMASK(5, FPGA_RPDVCR_TS_OFF)
#define FPGA_RPDVCR_FRMRUN  __BV(5)     // run frame data capture
#define FPGA_RPDVCR_TSRUN   __BV(6)     // run ts data capture
#define FPGA_RPDVCR_FRMRD   __BV(7)     // 0->1 reads next word from
                                     // frame data fifo                                  
#define FPGA_PRBSCR_TP_OFF  8   // prbs type
#define FPGA_PRBSCR_TP      BITMASK(4, FPGA_PRBSCR_TP_OFF)

#define FPGA_RPBTCR_INV     __BV(12)    // inversion of received sequence
#define FPGA_RPBTCR_RST     __BV(13)    // reset(1) rx bert
                                    // (required after UPATR* changing)

// UPATR1/TPPCR/ELFIFO
#define FPGA_UPATR1_OFF     0
#define FPGA_UPATR1_MASK    BITMASK(8, FPGA_UPATR1_OFF)
#define FPGA_TPPCR_INV      __BV(8)     // tx prbs inversion
#define FPGA_TPPCR_RST      __BV(9)     // reset(1), required if UPATR*/TPPCR changed
#define FPGA_ELFIFO_SCLR    __BV(10)    // reset fifo
#define FPGA_ELFIFO_WB_SHIFT (11)       // shift to size of fifo (bits)
#define FPGA_ELFIFO_WB0     __BV(11)    // size of fifo (bits)
#define FPGA_ELFIFO_WB2     __BV(13)    // 1=>32, 2=>64, 3=>128, 4=>256, other=>0
#define FPGA_ELFIFO_OFF     ( 0 << FPGA_ELFIFO_WB_SHIFT )       // off
#define FPGA_ELFIFO_32_BIT  ( 1 << FPGA_ELFIFO_WB_SHIFT )       // 32 bit
#define FPGA_ELFIFO_64_BIT  ( 2 << FPGA_ELFIFO_WB_SHIFT )       // 64 bit
#define FPGA_ELFIFO_128_BIT ( 3 << FPGA_ELFIFO_WB_SHIFT )       // 128 bit
#define FPGA_ELFIFO_256_BIT ( 4 << FPGA_ELFIFO_WB_SHIFT )       // 256 bit
#define FPGA_ELFIFO_WB_MASK BITMASK(3, FPGA_ELFIFO_WB_SHIFT)

// TPEGR/LICR/CVMSKD
#define FPGA_TPEGR_TP_OFF   0   // tx error or alarm type bits
#define FPGA_TPEGR_TP_MASK  BITMASK(5, FPGA_TPEGR_TP_OFF)
#define FPGA_TPEGR_RST      __BV(5)     // 1 - reset

#define FPGA_LICR_LOADRX    __BV(6)     //
#define FPGA_LICR_LOADSYNC  __BV(7)     //
#define FPGA_LICR_SHORTRX   __BV(8)     //

#define FPGA_CVMSKD_4ZERO   __BV(9)     // последовательность 4 и более нулей
#define FPGA_CVMSKD_2B      __BV(10)    // 2х импульсов одинаковой полярности
#define FPGA_CVMSKD_3B      __BV(11)    // последовательность 3х импульсов одинаковой полярности
#define FPGA_CVMSKD_4ZERR   __BV(12)    // ошибки счетности (неправильная замена 4х нулей)

// TPSIL/CVMSKG
#define FPGA_TPSIL_OFF      0
#define FPGA_TPSIL_MASK     BITMASK(8, FPGA_TPSIL_OFF)

#define FPGA_CVMSKG_4ZERO   __BV(8)     // последовательность 4 и более нулей
#define FPGA_CVMSKG_2BB     __BV(9)     // 2х импульсов одинаковой полярности BB
#define FPGA_CVMSKG_2B0B    __BV(10)    // 2х импульсов одинаковой полярности B0B
#define FPGA_CVMSKG_3B      __BV(11)    // последовательность 3х импульсов одинаковой полярности
#define FPGA_CVMSKG_4ZERR   __BV(12)    // ошибки счетности (неправильная замена 4х нулей)

// TPSISA4R
#define FPGA_TPSI_OFF       8
#define FPGA_TPSI_MASK      BITMASK(8, FPGA_TPSI_OFF)

#define FPGA_TPSA4_OFF      0
#define FPGA_TPSA4_MASK     BITMASK(8, FPGA_TPSA4_OFF)

// TPSA65R
#define FPGA_TPSA5_OFF      0
#define FPGA_TPSA5_MASK     BITMASK(8, FPGA_TPSA5_OFF)

#define FPGA_TPSA6_OFF      8
#define FPGA_TPSA6_MASK     BITMASK(8, FPGA_TPSA6_OFF)

// TPSA87R
#define FPGA_TPSA7_OFF      0
#define FPGA_TPSA7_MASK     BITMASK(8, FPGA_TPSA7_OFF)

#define FPGA_TPSA8_OFF      8
#define FPGA_TPSA8_MASK     BITMASK(8, FPGA_TPSA8_OFF)

// TPEGPR1/PCRTS
#define FPGA_TPEGPR1_OFF    0
#define FPGA_TPEGPR1_MASK   BITMASK(8, FPGA_TPEGPR1_OFF)

#define FPGA_PCRTS_OFF      8
#define FPGA_PCRTS_MASK     BITMASK(5, FPGA_PCRTS_OFF)
#define FPGA_PCRTS_EN       __BV(13)

// NCOPH1/JGPHR0
#define FPGA_NCOPH1_OFF     0
#define FPGA_NCOPH1_MASK    BITMASK(8, FPGA_NCOPH1_OFF)
#define FPGA_JGPHR0_OFF     8
#define FPGA_JGPHR0_MASK    BITMASK(8, FPGA_JGPHR0_OFF)

// TPEGCR
#define FPGA_TPEGCR_OFF     0
#define FPGA_TPEGCR_MASK    BITMASK(13, FPGA_TPEGCR_OFF)

// COPYCR/JGEND
                           // hold "1" to copy:
#define FPGA_COPYCR_JITM    __BV(0)     // measured jitter to buffer registers
#define FPGA_COPYCR_RMS     __BV(1)     // rms and freq values to buffer registers
#define FPGA_COPYCR_SLMCR   __BV(2)     // measured signal level to buffer
#define FPGA_COPYCR_FREQ    __BV(3)     // measured frequency to buffer
                           // perform "0->1" transition to copy:
#define FPGA_COPYCR_OSC     __BV(4)     // next oscilloscope's sample to buffer
#define FPGA_COPYCR_STR     __BV(5)     // next stroboscope's sample to buffer
#define FPGA_COPYCR_WND     __BV(6)     // next wander sample to buffer

#define FPGA_JGEND_OFF      8   // jittgen data bit #0, bit#7
#define FPGA_JGEND_MASK     BITMASK(8, FPGA_JGEND_OFF)

// JGENCR
#define FPGA_JGENCR_A_OFF   0
#define FPGA_JGENCR_A_MASK  BITMASK(5, FPGA_JGENCR_A_OFF)
#define FPGA_JGENCR_BE_OFF  5
#define FPGA_JGENCR_BE_MASK BITMASK(3, FPGA_JGENCR_BE_OFF)
#define FPGA_JGENCR_RST     __BV(8)     // hold "1" to reset jitter generator
#define FPGA_JGENCR_WR      __BV(9)     // make 0->1 to write JGEND[] to jitt gen

// KEYCR
#define FPGA_KEYCR_VAL_OFFS 0   //value of key
#define FPGA_KEYCR_VAL_MASK BITMASK(24,FPGA_KEYCR_VAL_OFFS)

#define FPGA_KEYCR_RST       __BV(8)    //make 0->1 to update value of key

/*
 * Status bits
 */
#define FPGA_FEAT_FWID_OFF      (0)
#define FPGA_FEAT_FWID_MASK     BITMASK(4, FPGA_FEAT_FWID_OFF)

#define FPGA_SR1_EGBSY          __BV(4) // error generator module busy
#define FPGA_SR1_PSBSY          __BV(5) // pulse shape busy
#define FPGA_SR1_RTDBSY         __BV(6) // RTD measurement busy

/*
 * FPGA_RPAR ( rx pcm alarms register )
 */
#define FPGA_ALARM_LOF          __BV(0)
#define FPGA_ALARM_RDI          __BV(1)
#define FPGA_ALARM_LMF          __BV(2)
#define FPGA_ALARM_LOM          __BV(3)
#define FPGA_ALARM_CASLOS       __BV(4)
#define FPGA_ALARM_CASAIS       __BV(5)
#define FPGA_ALARM_RMA          __BV(6)
#define FPGA_ALARM_BERLSS       __BV(7)
#define FPGA_ALARM_BERALL1      __BV(8)
#define FPGA_ALARM_BERALL0      __BV(9)
#define FPGA_ALARM_LOS          __BV(10)
#define FPGA_ALARM_AIS          __BV(11)
// clock slip negative
#define FPGA_ALARM_SLIPN        __BV(12)
// clock slip positive
#define FPGA_ALARM_SLIPP        __BV(13)

/*
 * FPGA_RPECR ( rx pcm error counters register )
 */
#define FPGA_ERR_CNTR_FAS           0   // FEC  (g.704)
#define FPGA_ERR_CNTR_CRC           1   // CRCE (g.704)
#define FPGA_ERR_CNTR_REBE          2   // REBE (g.704)
#define FPGA_ERR_CNTR_CAS_MFE       3   // cas multiframe error cntr
#define FPGA_ERR_CNTR_EBIT          4   // ber test's bit error counter
#define FPGA_ERR_CNTR_G826A         5   // g826 anomalies counter
#define FPGA_ERR_CNTR_MFE           6   // multiframe error counter
#define FPGA_ERR_CNTR_SLIPN         7   // negative pattern slips counter
#define FPGA_ERR_CNTR_SLIPP         8   // positive pattern slips counter
#define FPGA_ERR_CNTR_CODE          9   // code violation counter
#define FPGA_ERR_CNTR_CNT           10

/*
 * FPGA_TPEGR ( tx pcm error generator control register )
 */
#define EGEN_BIT_ERR   0
#define EGEN_EBIT_ERR  1
#define EGEN_FAS_ERR   2
#define EGEN_MFAS_ERR  3
#define EGEN_CRC_ERR   4
#define EGEN_REBE_ERR  5
#define EGEN_SLIPN_ERR 6
#define EGEN_SLIPP_ERR 7
#define EGEN_CODE_ERR  8

#define EGEN_ERRS_CNT 9

#define EGEN_LOF_ALARM  9
#define EGEN_LOM_ALARM  10
#define EGEN_RDI_ALARM  11
#define EGEN_LSS_ALARM  12
#define EGEN_ARTF_ALARM 13
#define EGEN_LOS_ALARM  14
#define EGEN_AIS_ALARM  15

#define EGEN_RMA_ALARM  16

#define EGEN_ALRMS_CNT  8

/*
 * pcm transmit clock source (FPGA_TPCR_TCSRC)
 *
 * NOTE: currently all prbs indexes below are coinsides
 * with indexes in FPGA. (it is so for e1pcm)
 */
#define FPGA_TCSRC_RX        0
#define FPGA_TCSRC_INT       1
#define FPGA_TCSRC_SYNC      2

void fpgaregs_init( void );
int fpga_registers( void );
int fpgaregs_rw( void );

#endif // FPGAREGS__H
