
/*
===============================================================================

This C header file is part of TestFloat, Release 2a, a package of programs
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

#ifndef __SYST_MODES_H__
#define __SYST_MODES_H__

#include "milieu.h"

/*
-------------------------------------------------------------------------------
Target-specific function for setting the system's IEC/IEEE floating-point
rounding mode.  Other system modes are also initialized as necessary (for
example, exception trapping may be disabled).
-------------------------------------------------------------------------------
*/
void syst_float_set_rounding_mode( int8 );

/*
-------------------------------------------------------------------------------
Target-specific function for setting the IEC/IEEE rounding precision of
subsequent extended double-precision operations performed by the system.
-------------------------------------------------------------------------------
*/
void syst_float_set_rounding_precision( int8 );

/*
-------------------------------------------------------------------------------
Prints fields from FPCSR
-------------------------------------------------------------------------------
*/
void syst_print_fpcsr_fields(void);

#endif // __SYST_MODES_H__