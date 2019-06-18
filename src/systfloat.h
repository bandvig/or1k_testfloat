
/* =====================================================================

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

===================================================================== */

#ifndef __SYST_FLOAT_H__
#define __SYST_FLOAT_H__

/* ---------------------------------------------------------------------
The following macros are defined to indicate that the corresponding
functions exist.
Single precision.
--------------------------------------------------------------------- */
#ifdef SYST_FLOAT32 // from GCC's command line
  #define SYST_INT32_TO_FLOAT32 // 1
  //#define SYST_FLOAT32_TO_INT32 // 9
  #define SYST_FLOAT32_TO_INT32_ROUND_TO_ZERO // 10
  //#define SYST_FLOAT32_ROUND_TO_INT // 16
  #define SYST_FLOAT32_ADD // 17
  #define SYST_FLOAT32_SUB // 18
  #define SYST_FLOAT32_MUL // 19
  #define SYST_FLOAT32_DIV // 20
  #define SYST_FLOAT32_EQ  // 23
  #define SYST_FLOAT32_LE  // 24
  #define SYST_FLOAT32_LT  // 25
#endif // SYST_FLOAT32

/* ---------------------------------------------------------------------
The following macros are defined to indicate that the corresponding
functions exist.
Double precision.
--------------------------------------------------------------------- */
#ifdef SYST_FLOAT64 // from GCC's command line
  #define SYST_INT64_TO_FLOAT64 //  6
  //#define SYST_FLOAT64_TO_INT64 // 31
  #define SYST_FLOAT64_TO_INT64_ROUND_TO_ZERO // 32
  #define SYST_FLOAT64_ADD // 37
  #define SYST_FLOAT64_SUB // 38
  #define SYST_FLOAT64_MUL // 39
  #define SYST_FLOAT64_DIV // 40
  #define SYST_FLOAT64_EQ  // 43
  #define SYST_FLOAT64_LE  // 44
  #define SYST_FLOAT64_LT  // 45
#endif // SYST_FLOAT64



/* ---------------------------------------------------------------------
System function declarations.  (Some of these functions may not exist.)
Single precision.
--------------------------------------------------------------------- */
float32 syst_int32_to_float32( int32 );
int32 syst_float32_to_int32( float32 );
int32 syst_float32_to_int32_round_to_zero( float32 );
float32 syst_float32_round_to_int( float32 );
float32 syst_float32_add( float32, float32 );
float32 syst_float32_sub( float32, float32 );
float32 syst_float32_mul( float32, float32 );
float32 syst_float32_div( float32, float32 );
float32 syst_float32_rem( float32, float32 );
flag syst_float32_eq( float32, float32 );
flag syst_float32_le( float32, float32 );
flag syst_float32_lt( float32, float32 );
flag syst_float32_eq_signaling( float32, float32 );
flag syst_float32_le_quiet( float32, float32 );
flag syst_float32_lt_quiet( float32, float32 );


/* ---------------------------------------------------------------------
System function declarations.  (Some of these functions may not exist.)
Double precision.
--------------------------------------------------------------------- */
float64 syst_int64_to_float64( int64 );
int64   syst_float64_to_int64( float64 );
int64   syst_float64_to_int64_round_to_zero( float64 );
float64 syst_float64_add( float64, float64 );
float64 syst_float64_sub( float64, float64 );
float64 syst_float64_mul( float64, float64 );
float64 syst_float64_div( float64, float64 );
flag    syst_float64_eq( float64, float64 );
flag    syst_float64_le( float64, float64 );
flag    syst_float64_lt( float64, float64 );

#endif // __SYST_FLOAT_H__
