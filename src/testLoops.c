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

#include <stdlib.h>
#include <stdio.h>

#include "softfloat.h"
#include "testCases.h"
#include "writeHex.h"
#include "testLoops.h"

extern void or1k_printf_fpcsr(void);
extern void syst_print_fpcsr_fields(void);
//extern void or1k_report(unsigned long int); // NewLIB 2.4.0+

volatile flag stop = FALSE;

char *trueName, *testName;
flag forever, errorStop;
uint32 maxErrorCount = 0;

flag checkNaNs = CHECK_NANs;

int8 *trueFlagsPtr;
int8 ( *testFlagsFunctionPtr )( void );

#ifdef ENABLE_FP_EXCEPTION
extern void testEnableFpException(void);
#endif

char *functionName;
char *roundingPrecisionName, *roundingModeName, *tininessModeName;
flag anyErrors = FALSE;

void writeFunctionName( void )
{
  printf( functionName );
  if ( roundingModeName )
  {
    if ( roundingPrecisionName )
      printf(", precision %s",roundingPrecisionName);
    printf(", rounding %s",roundingModeName);
    if ( tininessModeName )
      printf(", tininess %s",tininessModeName);
  }
  //syst_print_fpcsr_fields();
}

void exitWithStatus( void )
{
  exit( anyErrors ? 0x1 : 0 );
}

static uint32 tenthousandsCount, errorCount = 0;

static void writeTestsTotal( void )
{
    if ( forever ) {
        printf( "Unbounded tests.\r\n");
    }
    else {
        printf( "%d tests total.\r\n", testCases_total );
    }
}

static void writeTestsPerformed( int16 count )
{
    if ( tenthousandsCount )
    {
      printf("%d%04d tests performed",tenthousandsCount,count);
    }
    else
    {
      printf("%d tests performed",count);
    }

    if ( errorCount )
    {
      printf("; %d error%s found.\r\n",
                 errorCount,(( errorCount == 1 ) ? "" : "s"));
    }
    else
    {
        printf( ".\r\nNo errors found in ");
        writeFunctionName();
        printf( ".\r\n");
    }
}

static void
checkEarlyExit( void )
{
  ++tenthousandsCount;
  if ( stop )
  {
    writeTestsPerformed( 0 );
    exitWithStatus();
  }
  printf("%3d0000\r\n", tenthousandsCount);
}


static void writeErrorFound( int16 count )
{
  printf( "\r\n" );
  if ( errorCount == 1 )
  {
    printf( "Errors found in ");
    writeFunctionName();
    printf( ":\r\n");
  }

  if ( stop )
  {
    writeTestsPerformed( count );
    exitWithStatus();
  }
  else
    printf(" cnt: %d ", count);
  anyErrors = TRUE;
}


INLINE void writeInput_a_int32( void )
{
  printf("a_i32: ");
  writeHex_bits32( testCases_a_int32 );
}

#ifdef BITS64

INLINE void writeInput_a_int64( void )
{
  printf("a_i64: ");
  writeHex_bits64( testCases_a_int64 );
}

#endif

INLINE void writeInput_a_float32( void )
{
  printf("a_f32: ");
  writeHex_float32( testCases_a_float32);
}

static void writeInputs_ab_float32( void )
{
  printf("a_f32: ");
  writeHex_float32( testCases_a_float32 );
  printf("  b_f32: ");
  writeHex_float32( testCases_b_float32 );
}

INLINE void writeInput_a_float64( void )
{
  printf("a_f64: ");
  writeHex_float64( testCases_a_float64 );
}

static void writeInputs_ab_float64( void )
{
  printf("a_f64: ");
  writeHex_float64( testCases_a_float64 );
  printf("  b_f64: ");
  writeHex_float64( testCases_b_float64 );
}

#ifdef FLOATX80
INLINE void writeInput_a_floatx80( void )
{
  writeHex_floatx80( testCases_a_floatx80 );
}

static void writeInputs_ab_floatx80( void )
{
  writeHex_floatx80( testCases_a_floatx80 );
  printf( "  " );
  writeHex_floatx80( testCases_b_floatx80 );
}
#endif

#ifdef FLOAT128
INLINE void writeInput_a_float128( void )
{
  writeHex_float128( testCases_a_float128 );
}

static void writeInputs_ab_float128( void )
{
  writeHex_float128( testCases_a_float128 );
  printf( "  " );
  writeHex_float128( testCases_b_float128 );
}
#endif

static void
 writeOutputs_z_flag(
     flag trueZ, uint8 trueFlags, flag testZ, uint8 testFlags )
{

    printf( trueName );
    printf( ": " );
    writeHex_flag( trueZ );
    putchar( ' ' );
    writeHex_float_flags( trueFlags );
    printf( "  " );
    printf( testName );
    printf( ": " );
    writeHex_flag( testZ );
    putchar( ' ' );
    writeHex_float_flags( testFlags );
    printf("\r\n");

}

static void
 writeOutputs_z_int32(
     int32 trueZ, uint8 trueFlags, int32 testZ, uint8 testFlags )
{

    printf( trueName );
    printf( ": " );
    writeHex_bits32( trueZ );
    putchar( ' ' );
    writeHex_float_flags( trueFlags );
    printf( "  " );
    printf( testName );
    printf( ": " );
    writeHex_bits32( testZ );
    putchar( ' ' );
    writeHex_float_flags( testFlags );
    printf("\r\n");

}

#ifdef BITS64

static void
 writeOutputs_z_int64(
     int64 trueZ, uint8 trueFlags, int64 testZ, uint8 testFlags )
{

    printf( trueName );
    printf( ": " );
    writeHex_bits64( trueZ );
    putchar( ' ' );
    writeHex_float_flags( trueFlags );
    printf( "  " );
    printf( testName );
    printf( ": " );
    writeHex_bits64( testZ );
    putchar( ' ' );
    writeHex_float_flags( testFlags );
    printf("\r\n");

}

#endif


static void writeOutputs_z_float32(float32 trueZ, uint8 trueFlags,
                                   float32 testZ, uint8 testFlags)
{
  printf( trueName );
  printf( ": " );
  writeHex_float32( trueZ );
  putchar( ' ' );
  writeHex_float_flags( trueFlags );
  printf( "  " );
  printf( testName );
  printf( ": " );
  writeHex_float32( testZ );
  putchar( ' ' );
  writeHex_float_flags( testFlags );
  printf("\r\n");
}


static void writeOutputs_z_float64(float64 trueZ, uint8 trueFlags,
                                   float64 testZ, uint8 testFlags )
{
  printf( trueName );
  printf( ": " );
  writeHex_float64( trueZ );
  putchar( ' ' );
  writeHex_float_flags( trueFlags );
  printf( "  " );
  printf( testName );
  printf( ": " );
  writeHex_float64( testZ );
  putchar( ' ' );
  writeHex_float_flags( testFlags );
  printf("\r\n");
}

#ifdef FLOATX80

static void
 writeOutputs_z_floatx80(
     floatx80 trueZ, uint8 trueFlags, floatx80 testZ, uint8 testFlags )
{

    printf( trueName );
    printf( ": " );
    writeHex_floatx80( trueZ );
    putchar( ' ' );
    writeHex_float_flags( trueFlags );
    printf( "  " );
    printf( testName );
    printf( ": " );
    writeHex_floatx80( testZ );
    putchar( ' ' );
    writeHex_float_flags( testFlags );
    printf("\r\n");

}

#endif

#ifdef FLOAT128

static void
 writeOutputs_z_float128(
     float128 trueZ, uint8 trueFlags, float128 testZ, uint8 testFlags )
{

    printf( trueName );
    printf( ": " );
    writeHex_float128( trueZ );
    putchar( ' ' );
    writeHex_float_flags( trueFlags );
    printf( "\r\n\t" );
    printf( testName );
    printf( ": " );
    writeHex_float128( testZ );
    putchar( ' ' );
    writeHex_float_flags( testFlags );
    printf("\r\n");

}

#endif

INLINE flag float32_isANaN( float32 a )
{
  return (0x7F800000 < ( a & 0x7FFFFFFF ));
}

INLINE flag float32_isAnINF( float32 a )
{
  return (0x7F800000 == (a & 0x7FFFFFFF));
}

INLINE flag float32_isAMax( float32 a )
{
  return (0x7F7FFFFF == (a & 0x7FFFFFFF));
}

#ifdef BITS64

INLINE flag float64_same( float64 a, float64 b )
{

    return a == b;

}

INLINE flag float64_isNaN( float64 a )
{

    return (LIT64( 0x7FF0000000000000 ) < ( a & LIT64( 0x7FFFFFFFFFFFFFFF ) ));

}

INLINE flag float64_isAnINF( float64 a )
{
  return (LIT64(0x7FF0000000000000) == (a & LIT64(0x7FFFFFFFFFFFFFFF)));
}

INLINE flag float64_isAMax( float64 a )
{
  return (LIT64(0x7FEFFFFFFFFFFFFF) == (a & LIT64(0x7FFFFFFFFFFFFFFF)));
}

#else

INLINE flag float64_same( float64 a, float64 b )
{

    return ( a.high == b.high ) && ( a.low == b.low );

}

INLINE flag float64_isNaN( float64 a )
{
    bits32 absAHigh;

    absAHigh = a.high & 0x7FFFFFFF;
    return
        ( 0x7FF00000 < absAHigh ) || ( ( absAHigh == 0x7FF00000 ) && a.low );

}

#endif

#ifdef FLOATX80

INLINE flag floatx80_same( floatx80 a, floatx80 b )
{

    return ( a.high == b.high ) && ( a.low == b.low );

}

INLINE flag floatx80_isNaN( floatx80 a )
{

    return ( ( a.high & 0x7FFF ) == 0x7FFF ) && a.low;

}

#endif

#ifdef FLOAT128

INLINE flag float128_same( float128 a, float128 b )
{

    return ( a.high == b.high ) && ( a.low == b.low );

}

INLINE flag float128_isNaN( float128 a )
{
    bits64 absAHigh;

    absAHigh = a.high & LIT64( 0x7FFFFFFFFFFFFFFF );
    return
           ( LIT64( 0x7FFF000000000000 ) < absAHigh )
        || ( ( absAHigh == LIT64( 0x7FFF000000000000 ) ) && a.low );

}

#endif

void
 test_a_int32_z_float32(
     float32 trueFunction( int32 ), float32 testFunction( int32 ) )
{
    int16 count;
    float32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_isANaN( trueZ )
                 && float32_isANaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                printf( "  ");
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
       #if V_SHORT_TESTS==1
        // early test finish hack
        if (count == 9990) testCases_done = 1;
       #endif
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
}

void
 test_a_int32_z_float64(
     float64 trueFunction( int32 ), float64 testFunction( int32 ) )
{
    int16 count;
    float64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                printf( "  " );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
}

#ifdef FLOATX80

void
 test_a_int32_z_floatx80(
     floatx80 trueFunction( int32 ), floatx80 testFunction( int32 ) )
{
    int16 count;
    floatx80 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                printf( "  " );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_int32_z_float128(
     float128 trueFunction( int32 ), float128 testFunction( int32 ) )
{
    int16 count;
    float128 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                printf( "  " );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef BITS64

void
 test_a_int64_z_float32(
     float32 trueFunction( int64 ), float32 testFunction( int64 ) )
{
    int16 count;
    float32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_isANaN( trueZ )
                 && float32_isANaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                printf( "  " );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_a_int64_z_float64(
     float64 trueFunction( int64 ), float64 testFunction( int64 ) )
{
    int16 count;
    float64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                printf( "  " );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_int64_z_floatx80(
     floatx80 trueFunction( int64 ), floatx80 testFunction( int64 ) )
{
    int16 count;
    floatx80 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                printf( "  " );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_int64_z_float128(
     float128 trueFunction( int64 ), float128 testFunction( int64 ) )
{
    int16 count;
    float128 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                printf( "  " );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#endif

void
 test_a_float32_z_int32(
     int32 trueFunction( float32 ), int32 testFunction( float32 ) )
{
    int16 count;
    int32 trueZ, testZ;
    uint8 trueFlags, testFlags;
    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                printf( "  " );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
#if V_SHORT_TESTS==1
  // early test finish hack
  if (count == 9990) testCases_done = 1;
#endif
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_float32_z_int64(
     int64 trueFunction( float32 ), int64 testFunction( float32 ) )
{
    int16 count;
    int64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                printf( "  " );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_float32_z_float64(
     float64 trueFunction( float32 ), float64 testFunction( float32 ) )
{
    int16 count;
    float64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                printf( "  " );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_float32_z_floatx80(
     floatx80 trueFunction( float32 ), floatx80 testFunction( float32 ) )
{
    int16 count;
    floatx80 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                printf( "  " );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_float32_z_float128(
     float128 trueFunction( float32 ), float128 testFunction( float32 ) )
{
    int16 count;
    float128 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                printf( "  " );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_float32(
     float32 trueFunction( float32 ), float32 testFunction( float32 ) )
{
    int16 count;
    float32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float32_isANaN( trueZ )
                 && float32_isANaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                printf( "  " );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
#if V_SHORT_TESTS==1
  // early test finish hack
  if (count == 9990) testCases_done = 1;
#endif
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_float32_z_flag(
     flag trueFunction( float32, float32 ),
     flag testFunction( float32, float32 )
 )
{
    int16 count;
    flag trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32, testCases_b_float32 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
        //  printf("  a: %08X  b: %08X\r\n",testCases_a_float32,testCases_b_float32);
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float32, testCases_b_float32 );
        testFlags = testFlagsFunctionPtr();
        //  printf("  tZ: %d  tF: %d\r\n",testZ,testFlags);
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float32_is_signaling_nan( testCases_a_float32 )
                      || float32_is_signaling_nan( testCases_b_float32 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float32();
                printf( "  " );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
#if V_SHORT_TESTS==1
  // early test finish hack
  if (count == 9990) testCases_done = 1;
#endif
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void test_abz_float32(
     float32 trueFunction( float32, float32 ),
     float32 testFunction( float32, float32 ) )
{
  int16 count;
  float32 trueZ, testZ;
  uint8 trueFlags, testFlags;

  // ignore not sufficien differences in behavior of softfloat and IEEE
  flag ignoreNaNs, ignoreInfOut;

  errorCount = 0;
  tenthousandsCount = 0;
  count = 10000;
  testCases_initSequence( testCases_sequence_ab_float32 );
  writeTestsTotal();
  while ( !testCases_done || forever )
  {
    // next inputs
    testCases_next();

    // generate reference
    *trueFlagsPtr = 0;
    trueZ = trueFunction( testCases_a_float32, testCases_b_float32 );
    trueFlags = *trueFlagsPtr;

    // compute by system uder test
   #ifdef ENABLE_FP_EXCEPTION
    testEnableFpException();
   #endif
    testZ = testFunction( testCases_a_float32, testCases_b_float32 );
    testFlags = testFlagsFunctionPtr();

    // Indicate that tests are still going!
    //or1k_report(count); // NewLIB 2.4.0+
    --count;
    if ( count == 0 )
    {
      checkEarlyExit();
      count = 10000;
    }

    // check results
   #ifdef BLOCK_UNDERFLOW_FLAG
    if( (trueZ & 0x7FFFFFFF) == 0x00800000 )
    {
      trueFlags = trueFlags & (~float_flag_underflow);
      testFlags = testFlags & (~float_flag_underflow);
    }
   #endif

    if ( (trueZ != testZ) || (trueFlags != testFlags) )
    {
      // force invalid flag in case of an input is signaling NaN
      // and NaN check is off
      if ( !checkNaNs
           && ( float32_is_signaling_nan(testCases_a_float32)
                || float32_is_signaling_nan(testCases_b_float32) )
         )
      {
        trueFlags |= float_flag_invalid;
      }

      ignoreNaNs = ( !checkNaNs
                      && float32_isANaN(trueZ)
                      && float32_isANaN(testZ) );
      ignoreInfOut = ( (trueFlags & float_flag_overflow) && float32_isAMax(trueZ) &&
                       (testFlags & float_flag_overflow) && float32_isAnINF(testZ) );

      if ( ignoreNaNs || ignoreInfOut )
      {
        /* no problem */
      }
      else
      {
        ++errorCount;
        writeErrorFound( 10000 - count );
        writeInputs_ab_float32();
        printf( "  " );
        writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
        //fflush( stdout );
        if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
      }
    } // flags or results are not equal

   #if V_SHORT_TESTS==1
    // early test finish hack
    if (count == 9990) testCases_done = 1;
   #endif
  } // loop by test cases

 l_exit:
    writeTestsPerformed( 10000 - count );
    return;
}


void
 test_a_float64_z_int32(
     int32 trueFunction( float64 ), int32 testFunction( float64 ) )
{
    int16 count;
    int32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                printf( "  " );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_float64_z_int64(
     int64 trueFunction( float64 ), int64 testFunction( float64 ) )
{
    int16 count;
    int64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                printf( "  " );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_float64_z_float32(
     float32 trueFunction( float64 ), float32 testFunction( float64 ) )
{
    int16 count;
    float32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float32_isANaN( trueZ )
                 && float32_isANaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                printf( "  " );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_float64_z_floatx80(
     floatx80 trueFunction( float64 ), floatx80 testFunction( float64 ) )
{
    int16 count;
    floatx80 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                printf( "  " );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_float64_z_float128(
     float128 trueFunction( float64 ), float128 testFunction( float64 ) )
{
    int16 count;
    float128 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                printf( "  " );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_float64(
     float64 trueFunction( float64 ), float64 testFunction( float64 ) )
{
    int16 count;
    float64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                printf( "  " );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_float64_z_flag(
     flag trueFunction( float64, float64 ),
     flag testFunction( float64, float64 )
 )
{
    int16 count;
    flag trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64, testCases_b_float64 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float64, testCases_b_float64 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float64_is_signaling_nan( testCases_a_float64 )
                      || float64_is_signaling_nan( testCases_b_float64 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float64();
                printf( "  " );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void test_abz_float64(
     float64 trueFunction( float64, float64 ),
     float64 testFunction( float64, float64 ) )
{
  int16 count;
  float64 trueZ, testZ;
  uint8 trueFlags, testFlags;

  // ignore not sufficien differences in behavior of softfloat and IEEE
  flag ignoreNaNs, ignoreInfOut;

  errorCount = 0;
  tenthousandsCount = 0;
  count = 10000;
  testCases_initSequence( testCases_sequence_ab_float64 );
  writeTestsTotal();
  while ( ! testCases_done || forever )
  {
    // next inputs
    testCases_next();

    // generate reference
    *trueFlagsPtr = 0;
    trueZ = trueFunction( testCases_a_float64, testCases_b_float64 );
    trueFlags = *trueFlagsPtr;

    // compute by system uder test
   #ifdef ENABLE_FP_EXCEPTION
    testEnableFpException();
   #endif
    testZ = testFunction( testCases_a_float64, testCases_b_float64 );
    testFlags = testFlagsFunctionPtr();

    // progress counter
    --count;
    if ( count == 0 ) {
        checkEarlyExit();
        count = 10000;
    }

    // check results
   #ifdef BLOCK_UNDERFLOW_FLAG
    if( (trueZ & LIT64(0x7FFFFFFFFFFFFFFF)) == LIT64(0x0010000000000000) )
    {
      trueFlags = trueFlags & (~float_flag_underflow);
      testFlags = testFlags & (~float_flag_underflow);
    }
   #endif

    if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) )
    {
      if ( ! checkNaNs
            && (float64_is_signaling_nan( testCases_a_float64 )
                    || float64_is_signaling_nan( testCases_b_float64 ) )
         )
      {
        trueFlags |= float_flag_invalid;
      }

      /* original :
      if ( ! checkNaNs
           && float64_isNaN( trueZ )
           && float64_isNaN( testZ )
           && ! float64_is_signaling_nan( testZ )
           && ( trueFlags == testFlags )
         )
      {
              // no problem
      } */

      ignoreNaNs = ( !checkNaNs
                      && float64_isNaN(trueZ)
                      && float64_isNaN(testZ) );
      ignoreInfOut = ( (trueFlags & float_flag_overflow) && float64_isAMax(trueZ) &&
                       (testFlags & float_flag_overflow) && float64_isAnINF(testZ) );

      if ( ignoreNaNs || ignoreInfOut )
      {
        /* no problem */
      }
      else
      {
        ++errorCount;
        writeErrorFound( 10000 - count );
        writeInputs_ab_float64();
        printf( "  " );
        writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
        //fflush( stdout );
        if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
      }
    }
  }
l_exit:
  writeTestsPerformed( 10000 - count );
  return;
}

#ifdef FLOATX80

void
 test_a_floatx80_z_int32(
     int32 trueFunction( floatx80 ), int32 testFunction( floatx80 ) )
{
    int16 count;
    int32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                printf( "  " );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_floatx80_z_int64(
     int64 trueFunction( floatx80 ), int64 testFunction( floatx80 ) )
{
    int16 count;
    int64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                printf( "  " );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_floatx80_z_float32(
     float32 trueFunction( floatx80 ), float32 testFunction( floatx80 ) )
{
    int16 count;
    float32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float32_isANaN( trueZ )
                 && float32_isANaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                printf( "  " );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_a_floatx80_z_float64(
     float64 trueFunction( floatx80 ), float64 testFunction( floatx80 ) )
{
    int16 count;
    float64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                printf( "  " );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOAT128

void
 test_a_floatx80_z_float128(
     float128 trueFunction( floatx80 ), float128 testFunction( floatx80 ) )
{
    int16 count;
    float128 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                printf( "  " );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_floatx80(
     floatx80 trueFunction( floatx80 ), floatx80 testFunction( floatx80 ) )
{
    int16 count;
    floatx80 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                printf( "  " );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_floatx80_z_flag(
     flag trueFunction( floatx80, floatx80 ),
     flag testFunction( floatx80, floatx80 )
 )
{
    int16 count;
    flag trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80, testCases_b_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80, testCases_b_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    floatx80_is_signaling_nan( testCases_a_floatx80 )
                      || floatx80_is_signaling_nan( testCases_b_floatx80 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_floatx80();
                printf( "  " );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void
 test_abz_floatx80(
     floatx80 trueFunction( floatx80, floatx80 ),
     floatx80 testFunction( floatx80, floatx80 )
 )
{
    int16 count;
    floatx80 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80, testCases_b_floatx80 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_floatx80, testCases_b_floatx80 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    floatx80_is_signaling_nan( testCases_a_floatx80 )
                      || floatx80_is_signaling_nan( testCases_b_floatx80 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_floatx80();
                printf( "  " );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
    return;

}

#endif

#ifdef FLOAT128

void
 test_a_float128_z_int32(
     int32 trueFunction( float128 ), int32 testFunction( float128 ) )
{
    int16 count;
    int32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                printf( "  " );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_float128_z_int64(
     int64 trueFunction( float128 ), int64 testFunction( float128 ) )
{
    int16 count;
    int64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                printf( "  " );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_float128_z_float32(
     float32 trueFunction( float128 ), float32 testFunction( float128 ) )
{
    int16 count;
    float32 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float32_isANaN( trueZ )
                 && float32_isANaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                printf( "  " );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_a_float128_z_float64(
     float64 trueFunction( float128 ), float64 testFunction( float128 ) )
{
    int16 count;
    float64 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                printf( "  " );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_float128_z_floatx80(
     floatx80 trueFunction( float128 ), floatx80 testFunction( float128 ) )
{
    int16 count;
    floatx80 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                printf( "  " );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_float128(
     float128 trueFunction( float128 ), float128 testFunction( float128 ) )
{
    int16 count;
    float128 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                printf( "  " );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_float128_z_flag(
     flag trueFunction( float128, float128 ),
     flag testFunction( float128, float128 )
 )
{
    int16 count;
    flag trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128, testCases_b_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128, testCases_b_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float128_is_signaling_nan( testCases_a_float128 )
                      || float128_is_signaling_nan( testCases_b_float128 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float128();
                printf( "  " );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void
 test_abz_float128(
     float128 trueFunction( float128, float128 ),
     float128 testFunction( float128, float128 )
 )
{
    int16 count;
    float128 trueZ, testZ;
    uint8 trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        // next inputs
        testCases_next();
        // generate reference
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128, testCases_b_float128 );
        trueFlags = *trueFlagsPtr;
        // compute by system uder test
       #ifdef ENABLE_FP_EXCEPTION
        testEnableFpException();
       #endif
        testZ = testFunction( testCases_a_float128, testCases_b_float128 );
        testFlags = testFlagsFunctionPtr();
        // progress counter
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        // check results
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float128_is_signaling_nan( testCases_a_float128 )
                      || float128_is_signaling_nan( testCases_b_float128 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if (    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float128();
                printf( "  " );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                //fflush( stdout );
                if ( errorCount == maxErrorCount ) exit(1); // goto l_exit;
            }
        }
    }
 l_exit:
    writeTestsPerformed( 10000 - count );
    return;

}

#endif

