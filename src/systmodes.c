
/*
===============================================================================

This C source file is part of TestFloat, Release 2a, a package of programs
for testing the correctness of floating-point arithmetic complying to the
IEC/IEEE Standard for Floating-Point.

Written by John R. Hauser.  More information is available through the Web
page `http://HTTP.CS.Berkeley.EDU/~jhauser/arithmetic/TestFloat.html'.

THIS SOFTWARE IS DISTRIBUTED AS IS, FOR FREE.  Although reasonable effort
has been made to avoid it, THIS SOFTWARE MAY CONTAIN FAULTS THAT WILL AT
TIMES RESULT IN INCORRECT BEHAVIOR.  USE OF THIS SOFTWARE IS RESTRICTED TO
PERSONS AND ORGANIZATIONS WHO CAN AND WILL TAKE FULL RESPONSIBILITY FOR ANY
AND ALL LOSSES, COSTS, OR OTHER PROBLEMS ARISING FROM ITS USE.

Derivative works are acceptable, even for commercial purposes, so long as
(1) they include prominent notice that the work is derivative, and (2) they
include prominent notice akin to these four paragraphs for those parts of
this code that are retained.

Modified for use with or1ksim's testsuite.

Contributor Julius Baxter <julius.baxter@orsoc.se>

===============================================================================
*/

#include <stdio.h>

// OR1k support C library
#include <or1k-support.h>
//#include <spr-defs.h> // NewLIB 2.0.0
#include <or1k-sprs.h>  // NewLIB 2.4.0+

#include "systmodes.h"

/*
-------------------------------------------------------------------------------
Sets the system's IEC/IEEE floating-point rounding mode.  Also disables all
system exception traps.
-------------------------------------------------------------------------------
*/
void syst_float_set_rounding_mode( int8 roundingMode )
{
  // Read the FPCSR
  //unsigned int value = (unsigned int)or1k_mfspr(SPR_FPCSR); // NewLIB 2.0.0
  unsigned int value = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_FPCSR_ADDR); // NewLIB 2.4.0+

  // Clear the current rounding mode
  //value &= ~SPR_FPCSR_RM; // NewLIB 2.0.0

  // Extract the flags from OR1K's FPCSR, put into testfloat's flags format
  switch(roundingMode)
    {
    case float_round_nearest_even:
      //value |= FPCSR_RM_RN; // NewLIB 2.0.0
      value = OR1K_SPR_SYS_FPCSR_RM_SET(value,OR1K_SPR_SYS_FPCSR_RM_NEAREST); // NewLIB 2.4.0+
      break;
    case float_round_down:
      //value |= FPCSR_RM_RIN; // NewLIB 2.0.0
      value = OR1K_SPR_SYS_FPCSR_RM_SET(value,OR1K_SPR_SYS_FPCSR_RM_INFMINUS); // NewLIB 2.4.0+
      break;
    case float_round_up:
      //value |= FPCSR_RM_RIP; // NewLIB 2.0.0
      value = OR1K_SPR_SYS_FPCSR_RM_SET(value,OR1K_SPR_SYS_FPCSR_RM_INFPLUS); // NewLIB 2.4.0+
      break;
    case float_round_to_zero:
      //value |= FPCSR_RM_RZ; // NewLIB 2.0.0
      value = OR1K_SPR_SYS_FPCSR_RM_SET(value,OR1K_SPR_SYS_FPCSR_RM_ZERO); // NewLIB 2.4.0+
      break;
    default:
      //printf("%s: Unknown rounding mode: 0x%x\r\n",__FUNCTION__,roundingMode);
      // error!
      break;
    }

  // Write value back to FPCSR
  //or1k_mtspr(SPR_FPCSR,value); // NewLIB 2.0.0
  or1k_mtspr(OR1K_SPR_SYS_FPCSR_ADDR,value); // NewLIB 2.4.0+
}

/*
-------------------------------------------------------------------------------
Prints fields from FPCSR
-------------------------------------------------------------------------------
*/
void syst_print_fpcsr_fields(void)
{
  // Read the FPCSR
  /* NewLIB 2.0.0
  unsigned int value = (unsigned int)or1k_mfspr(SPR_FPCSR);
  printf(", FPCSR F:");
  putchar( value & SPR_FPCSR_IVF ? 'v' : '.' ); // Invalid Flag
  putchar( value & SPR_FPCSR_DZF ? 'z' : '.' ); // Divide By Zero Flag
  putchar( value & SPR_FPCSR_OVF ? 'o' : '.' ); // Overflow Flag
  putchar( value & SPR_FPCSR_UNF ? 'u' : '.' ); // Underflow Flag
  putchar( value & SPR_FPCSR_IXF ? 'x' : '.' ); // Inexact Flag
  printf(" RM:%1d E:%1d ",((value&SPR_FPCSR_RM)>>1),value&SPR_FPCSR_FPEE);
  */
  /* NewLIB 2.4.0+ */
  unsigned int value = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_FPCSR_ADDR);
  printf(", FPCSR F:");
  putchar( OR1K_SPR_SYS_FPCSR_IVF_GET(value) ? 'v' : '.' ); // Invalid Flag
  putchar( OR1K_SPR_SYS_FPCSR_DZF_GET(value) ? 'z' : '.' ); // Divide By Zero Flag
  putchar( OR1K_SPR_SYS_FPCSR_OVF_GET(value) ? 'o' : '.' ); // Overflow Flag
  putchar( OR1K_SPR_SYS_FPCSR_UNF_GET(value) ? 'u' : '.' ); // Underflow Flag
  putchar( OR1K_SPR_SYS_FPCSR_IXF_GET(value) ? 'x' : '.' ); // Inexact Flag
  printf(" RM:%1d E:%1d ",OR1K_SPR_SYS_FPCSR_RM_GET(value),OR1K_SPR_SYS_FPCSR_FPEE_GET(value));
}

/*
-------------------------------------------------------------------------------
Sets the rounding precision of subsequent extended double-precision
operations.  The `precision' argument should be one of 0, 32, 64, or 80.
If `precision' is 32, the rounding precision is set equivalent to single
precision; else if `precision' is 64, the rounding precision is set
equivalent to double precision; else the rounding precision is set to full
extended double precision.
-------------------------------------------------------------------------------
*/
void syst_float_set_rounding_precision( int8 precision )
{

  //!!!code (possibly empty)
  // Yes empty for OR1K 32-bit implementation - have no choice of rounding
  // precision.
  return;

}



