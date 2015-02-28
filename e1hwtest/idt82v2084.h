#ifndef CHIP_IDT82V2084__H
#define CHIP_IDT82V2084__H

/*
 * include/chip/idt82v2084.h
 * 
 * IDT 82V2084 
 * Quad Channel T1/E1/J1 Long Haul/Short Haul 
 * Line Interface Unit
 * 
 * Registers and bits definitions.
 * Useful defines.
 *
 * Naming convention.
 * AA_BB_CC
 *
 * AA - chip name
 * BB - register name
 * CC - bit name.
 */
#include <common/defs.h>

#define IDT82V2084_PORTS_CNT    4

// global register
#define IDT82V2084_GREG(x)      (x) 

// port register, p = 0,1..3
#define IDT82V2084_PREG(x, p)   ((x) + ((p) * 0x40))

/** Global registers
 */
#define IDT82V2084_ID           IDT82V2084_GREG(0x00)
#define IDT82V2084_RST          IDT82V2084_GREG(0x20)
#define IDT82V2084_GCF0         IDT82V2084_GREG(0x40)
#define IDT82V2084_GCF1         IDT82V2084_GREG(0x60)
#define IDT82V2084_INTCH        IDT82V2084_GREG(0x80)

/** Global registers' bits
 */
#define IDT82V2084_GCF0_T1E1    __BV(4)
#define IDT82V2084_GCF0_COPY    __BV(3)
#define IDT82V2084_GCF0_INTM_GLB __BV(2)

#define IDT82V2084_GCF0_INT_PIN_OFF 0
#define IDT82V2084_GCF0_INT_PIN BITMASK(2, \
                                IDT82V2084_GCF0_INT_PIN_OFF)

#define IDT82V2084_GCF1_MON_OFF 4 
#define IDT82V2084_GCF1_MON     BITMASK(4, \
                                IDT82V2084_GCF1_MON_OFF)

/** ports individual registers
 */
#define IDT82V2084_JACF(p)      IDT82V2084_PREG(0x01, p)
#define IDT82V2084_TCF0(p)      IDT82V2084_PREG(0x02, p)
#define IDT82V2084_TCF1(p)      IDT82V2084_PREG(0x03, p)
#define IDT82V2084_TCF2(p)      IDT82V2084_PREG(0x04, p)
#define IDT82V2084_TCF3(p)      IDT82V2084_PREG(0x05, p)
#define IDT82V2084_TCF4(p)      IDT82V2084_PREG(0x06, p)
#define IDT82V2084_RCF0(p)      IDT82V2084_PREG(0x07, p)
#define IDT82V2084_RCF1(p)      IDT82V2084_PREG(0x08, p)
#define IDT82V2084_RCF2(p)      IDT82V2084_PREG(0x09, p)
#define IDT82V2084_MAINT0(p)    IDT82V2084_PREG(0x0a, p)
#define IDT82V2084_MAINT1(p)    IDT82V2084_PREG(0x0b, p)
#define IDT82V2084_MAINT2(p)    IDT82V2084_PREG(0x0c, p)
#define IDT82V2084_MAINT3(p)    IDT82V2084_PREG(0x0d, p)
#define IDT82V2084_MAINT4(p)    IDT82V2084_PREG(0x0e, p)
#define IDT82V2084_MAINT5(p)    IDT82V2084_PREG(0x0f, p)
#define IDT82V2084_MAINT6(p)    IDT82V2084_PREG(0x10, p)
#define IDT82V2084_INTM0(p)     IDT82V2084_PREG(0x11, p)
#define IDT82V2084_INTM1(p)     IDT82V2084_PREG(0x12, p)
#define IDT82V2084_INTES(p)     IDT82V2084_PREG(0x13, p)
#define IDT82V2084_STAT0(p)     IDT82V2084_PREG(0x14, p)
#define IDT82V2084_STAT1(p)     IDT82V2084_PREG(0x15, p)
#define IDT82V2084_INTS0(p)     IDT82V2084_PREG(0x16, p)
#define IDT82V2084_INTS1(p)     IDT82V2084_PREG(0x17, p)
#define IDT82V2084_CNT0(p)      IDT82V2084_PREG(0x18, p)
#define IDT82V2084_CNT1(p)      IDT82V2084_PREG(0x19, p)
#define IDT82V2084_TERM(p)      IDT82V2084_PREG(0x1A, p)

#define IDT82V2084_MEM_SIZE     (IDT82V2084_TERM(IDT82V2084_PORTS_CNT-1) + 1)

/** ports individual registers' bits definitions
 */
// jitter attenuator configuration
#define IDT82V2084_JACF_JA_LIMIT   __BV(5)
#define IDT82V2084_JACF_JACF_OFF   3
#define IDT82V2084_JACF_JACF       BITMASK(2, \
                                   IDT82V2084_JACF_JACF_OFF)
#define IDT82V2084_JACF_JADP_OFF   1
#define IDT82V2084_JACF_JADP       BITMASK(2, \
                                   IDT82V2084_JACF_JADP_OFF)
#define IDT82V2084_JACF_JABW       0

// transmitter configuration
#define IDT82V2084_TCF0_T_OFF      __BV(4)
#define IDT82V2084_TCF0_TD_INV     __BV(3)
#define IDT82V2084_TCF0_TCLK_SEL   __BV(2)
#define IDT82V2084_TCF0_T_MD_OFF   0
#define IDT82V2084_TCF0_T_MD       BITMASK(2, \
                                   IDT82V2084_TCF0_T_MD_OFF)

#define IDT82V2084_TCF1_DRM_OFF    __BV(5)
#define IDT82V2084_TCF1_THZ        __BV(4)
#define IDT82V2084_TCF1_PULS_OFF   0
#define IDT82V2084_TCF1_PULS       BITMASK(4, \
                                   IDT82V2084_TCF1_PULS_OFF)

#define IDT82V2084_TCF2_SCAL_OFF   0
#define IDT82V2084_TCF2_SCAL       BITMASK(6, \
                                   IDT82V2084_TCF2_SCAL_OFF)

#define IDT82V2084_TCF3_DONE       __BV(7)
#define IDT82V2084_TCF3_RW         __BV(6)
#define IDT82V2084_TCF3_UI_OFF     4
#define IDT82V2084_TCF3_UI         BITMASK(2, \
                                   IDT82V2084_TCF3_UI_OFF)
#define IDT82V2084_TCF3_SAMP_OFF   0
#define IDT82V2084_TCF3_SAMP       BITMASK(4, \
                                   IDT82V2084_TCF3_SAMP_OFF)

#define IDT82V2084_TCF4_WDAT_OFF   0
#define IDT82V2084_TCF4_WDAT       BITMASK(7, \
                                   IDT82V2084_TCF4_WDAT_OFF)

// receiver configuration
#define IDT82V2084_RCF0_R_OFF      __BV(4)
#define IDT82V2084_RCF0_RD_INV     __BV(3)
#define IDT82V2084_RCF0_RCLK_SEL   __BV(2)
#define IDT82V2084_RCF0_R_MD_OFF   0
#define IDT82V2084_RCF0_R_MD       BITMASK(2, \
                                   IDT82V2084_RCF0_R_MD_OFF)

#define IDT82V2084_RCF1_EQ_ON      __BV(6)
#define IDT82V2084_RCF1_LOS_OFF    0
#define IDT82V2084_RCF1_LOS        BITMASK(5, \
                                   IDT82V2084_RCF1_LOS_OFF)

#define IDT82V2084_RCF2_SLICE_OFF  4
#define IDT82V2084_RCF2_SLICE      BITMASK(2, \
                                   IDT82V2084_RCF2_SLICE_OFF)
#define IDT82V2084_RCF2_UPDW_OFF   2
#define IDT82V2084_RCF2_UPDW       BITMASK(2, \
                                   IDT82V2084_RCF2_UPDW_OFF)
#define IDT82V2084_RCF2_MG_OFF     0
#define IDT82V2084_RCF2_MG         BITMASK(2, \
                                   IDT82V2084_RCF2_MG_OFF)

// maintenance functions
#define IDT82V2084_MAINT0_PATT_OFF 5
#define IDT82V2084_MAINT0_PATT     BITMASK(2, \
                                   IDT82V2084_MAINT0_PATT_OFF)
#define IDT82V2084_MAINT0_PATT_CLK __BV(4)
#define IDT82V2084_MAINT0_PRBS_INV __BV(3)
#define IDT82V2084_MAINT0_LAC      __BV(2)
#define IDT82V2084_MAINT0_AISE     __BV(1)
#define IDT82V2084_MAINT0_ATAO     __BV(0)

#define IDT82V2084_MAINT1_ARLP     __BV(3)
#define IDT82V2084_MAINT1_RLP      __BV(2)
#define IDT82V2084_MAINT1_ALP      __BV(1)
#define IDT82V2084_MAINT1_DLP      __BV(0)

#define IDT82V2084_MAINT2_TIBLB_L_OFF  4
#define IDT82V2084_MAINT2_TIBLB_L  BITMASK(2, \
                                   IDT82V2084_MAINT2_TIBLB_L_OFF)
#define IDT82V2084_MAINT2_RIBLBA_L_OFF 2
#define IDT82V2084_MAINT2_RIBLBA_L BITMASK(2, \
                                   IDT82V2084_MAINT2_RIBLBA_L_OFF)
#define IDT82V2084_MAINT2_RIBLBD_L_OFF 0
#define IDT82V2084_MAINT2_RIBLBD_L BITMASK(2, \
                                   IDT82V2084_MAINT2_RIBLBD_L_OFF)

#define IDT82V2084_MAINT3_TIBLB_OFF  0
#define IDT82V2084_MAINT3_TIBLB    BITMASK(8, \
                                   IDT82V2084_MAINT3_TIBLB_OFF)

#define IDT82V2084_MAINT4_RIBLBA_OFF  0
#define IDT82V2084_MAINT4_RIBLBA   BITMASK(8, \
                                   IDT82V2084_MAINT4_RIBLBA_OFF)

#define IDT82V2084_MAINT5_RIBLBD_OFF  0
#define IDT82V2084_MAINT5_RIBLBD   BITMASK(8, \
                                   IDT82V2084_MAINT5_RIBLBD_OFF)

#define IDT82V2084_MAINT6_BPV_INS  __BV(6)
#define IDT82V2084_MAINT6_ERR_INS  __BV(5)
#define IDT82V2084_MAINT6_EXZ_DEF  __BV(4)
#define IDT82V2084_MAINT6_ERR_SEL_OFF 2
#define IDT82V2084_MAINT6_ERR_SEL  BITMASK(2, \
                                   IDT82V2084_MAINT6_ERR_SEL_OFF)
#define IDT82V2084_MAINT6_CNT_MD   __BV(1)
#define IDT82V2084_MAINT6_CMT_TRF  __BV(0)

// interrupt control registers
#define IDT82V2084_INTM0_EQ_IM     __BV(7)
#define IDT82V2084_INTM0_IBLBA_IM  __BV(6)
#define IDT82V2084_INTM0_IBLBD_IM  __BV(5)
#define IDT82V2084_INTM0_PRBS_IM   __BV(4)
#define IDT82V2084_INTM0_TCLK_IM   __BV(3)
#define IDT82V2084_INTM0_DF_IM     __BV(2)
#define IDT82V2084_INTM0_AIS_IM    __BV(1)
#define IDT82V2084_INTM0_LOS_IM    __BV(0)

#define IDT82V2084_INTM1_DAC_OV_IM __BV(7)
#define IDT82V2084_INTM1_JAOV_IM   __BV(6)
#define IDT82V2084_INTM1_JAUD_IM   __BV(5)
#define IDT82V2084_INTM1_ERR_IM    __BV(4)
#define IDT82V2084_INTM1_EXZ_IM    __BV(3)
#define IDT82V2084_INTM1_CV_IM     __BV(2)
#define IDT82V2084_INTM1_TIMER_IM  __BV(1)
#define IDT82V2084_INTM1_CNT_IM    __BV(0)

#define IDT82V2084_INTES_EQ_IES    __BV(7)
#define IDT82V2084_INTES_IBLBA_IES __BV(6)
#define IDT82V2084_INTES_IBLBD_IES __BV(5)
#define IDT82V2084_INTES_PRBS_IES  __BV(4)
#define IDT82V2084_INTES_TCLK_IES  __BV(3)
#define IDT82V2084_INTES_DF_IES    __BV(2)
#define IDT82V2084_INTES_AIS_IES   __BV(1)
#define IDT82V2084_INTES_LOS_IES   __BV(0)

// line status registers
#define IDT82V2084_STAT0_EQ_S      __BV(7)
#define IDT82V2084_STAT0_IBLBA_S   __BV(6)
#define IDT82V2084_STAT0_IBLBD_S   __BV(5)
#define IDT82V2084_STAT0_PRBS_S    __BV(4)
#define IDT82V2084_STAT0_TCLK_LOS  __BV(3)
#define IDT82V2084_STAT0_DF_S      __BV(2)
#define IDT82V2084_STAT0_AIS_S     __BV(1)
#define IDT82V2084_STAT0_LOS_S     __BV(0)

#define IDT82V2084_STAT1_RLP_S     __BV(5)
#define IDT82V2084_STAT1_LATT_OFF  0
#define IDT82V2084_STAT1_LATT      BITMASK(5, \
                                   IDT82V2084_STAT1_LATT_OFF)

// interrupt status registers
#define IDT82V2084_INTS0_EQ_IS     __BV(7)
#define IDT82V2084_INTS0_IBLBA_IS  __BV(6)
#define IDT82V2084_INTS0_IBLBD_IS  __BV(5)
#define IDT82V2084_INTS0_PRBS_IS   __BV(4)
#define IDT82V2084_INTS0_TCLK_LOS_IS __BV(3)
#define IDT82V2084_INTS0_DF_IS     __BV(2)
#define IDT82V2084_INTS0_AIS_IS    __BV(1)
#define IDT82V2084_INTS0_LOS_IS    __BV(0)

#define IDT82V2084_INTS1_DAC_OV_IS __BV(7)
#define IDT82V2084_INTS1_JAOV_IS   __BV(6)
#define IDT82V2084_INTS1_JAUD_IS   __BV(5)
#define IDT82V2084_INTS1_ERR_IS    __BV(4)
#define IDT82V2084_INTS1_EXZ_IS    __BV(3)
#define IDT82V2084_INTS1_CV_IS     __BV(2)
#define IDT82V2084_INTS1_TMOV_IS   __BV(1)
#define IDT82V2084_INTS1_CNT_OV_IS __BV(0)

// counters' registers
#define IDT82V2084_CNT_L_OFF    0
#define IDT82V2084_CNT_L        BITMASK(8, \
                                IDT82V2084_CNT_L_OFF)

#define IDT82V2084_CNT_H_OFF    0
#define IDT82V2084_CNT_H        BITMASK(8, \
                                IDT82V2084_CNT_H_OFF)

#define IDT82V2084_T_TERM_OFF   3
#define IDT82V2084_T_TERM       BITMASK(3, \
                                IDT82V2084_T_TERM_OFF)

#define IDT82V2084_R_TERM_OFF   0
#define IDT82V2084_R_TERM       BITMASK(3, \
                                IDT82V2084_R_TERM_OFF)

#endif // CHIP_IDT82V2084__H

