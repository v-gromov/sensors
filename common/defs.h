#ifndef DEFS__H
#define DEFS__H

/*
 * SET_BIT, GET_BIT 
 */
#define __BV(bit)                (0x1UL << (bit))
#define BITMASK(width, off)     (((1 << (width)) - 1) << (off))

#define SET_BIT(x, bit) \
  do{\
    (x) |= __BV(bit);\
  } while(0);

#define CLR_BIT(x, bit) \
  do{\
    (x) &= ~__BV(bit);\
  } while(0);

#define GET_BIT(x, bit) (((x) >> (bit)) & 0x1)

#define SET_BIT_DEP(x, bit, dep) \
  do {\
    if (dep) SET_BIT(x, bit);\
    else     CLR_BIT(x, bit);\
  } while (0)

#endif // DEFS__H
