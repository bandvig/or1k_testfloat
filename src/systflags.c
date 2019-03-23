
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

#include <or1k-support.h>  // NewLIB 2.4.0+
//#include <spr-defs.h> // NewLIB 2.0.0
#include <or1k-sprs.h>  // NewLIB 2.4.0+

#include "milieu.h"


/*
-------------------------------------------------------------------------------
Reads flags from FPCSR directly and convert to SoftFloat format.
-------------------------------------------------------------------------------
*/

int8 syst_float_flags_read( void )
{
  //unsigned int v_sr;
  unsigned int fpcsr;
  int8         flags;

  flags = 0;

  /* NewLIB 2.0.0
  // Read the SPR
  fpcsr = (unsigned int)or1k_mfspr(SPR_FPCSR);

  //v_sr  = (unsigned int)or1k_mfspr(SPR_SR);
  //  printf("  Read FPCSR: %08X  SM=%1d\r\n", fpcsr,(v_sr&1));

  // Extract the flags from OR1K's FPCSR, put into testfloat's flags format
  if (fpcsr & SPR_FPCSR_IXF)
    flags |= float_flag_inexact;

  if (fpcsr & SPR_FPCSR_UNF)
    flags |= float_flag_underflow;

  if (fpcsr & SPR_FPCSR_OVF)
    flags |= float_flag_overflow;

  if (fpcsr & SPR_FPCSR_DZF)
    flags |= float_flag_divbyzero;

  if (fpcsr & SPR_FPCSR_IVF)
    flags |= float_flag_invalid;

  // drop FP-Exception-Enabled and write back to FPCSR
  fpcsr = fpcsr & (~SPR_FPCSR_FPEE);
  or1k_mtspr(SPR_FPCSR,fpcsr);
  */

  // NewLIB 2.4.0+
  // Read the SPR
  fpcsr = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_FPCSR_ADDR);

  //v_sr  = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_SR_ADDR);
  //  printf("  FPCSR: %08X  SM=%1d\r\n", fpcsr,(v_sr&1));

  // Extract the flags from OR1K's FPCSR, put into testfloat's flags format
  if (OR1K_SPR_SYS_FPCSR_IXF_GET(fpcsr) != 0)
    flags |= float_flag_inexact;

  if (OR1K_SPR_SYS_FPCSR_UNF_GET(fpcsr) != 0)
    flags |= float_flag_underflow;

  if (OR1K_SPR_SYS_FPCSR_OVF_GET(fpcsr) != 0)
    flags |= float_flag_overflow;

  if (OR1K_SPR_SYS_FPCSR_DZF_GET(fpcsr) != 0)
    flags |= float_flag_divbyzero;

  if (OR1K_SPR_SYS_FPCSR_IVF_GET(fpcsr) != 0)
    flags |= float_flag_invalid;

  return flags;
}
