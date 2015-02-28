/** \file udelay.c
  * \brief Home of the udelay function
  * \author Nikolaj Zamotaev
  * \author STMicroelectronics
  */
#include <stdint.h>


/** \brief Delays for specified number of microseconds
  * \param usec -- number of microseconds to delay
  * \todo change to real timebase
  */
void udelay(const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = usec * 10;
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}
