
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
//#include <signal.h>
#include <string.h>

// OR1k support C library
#include <or1k-support.h>
//#include <spr-defs.h> // NewLIB 2.0.0
#include <or1k-sprs.h>  // NewLIB 2.4.0+

#include "test_float_fail.h"
#include "softfloat.h"
#include "testCases.h"
#include "testLoops.h"
#include "systflags.h"
#include "testFunction.h"



// enables "floating point exeption"
void testEnableFpException(void)
{
  // Read the FPCSR
  /* NewLIB 2.0.0
  unsigned int value = (unsigned int)or1k_mfspr(SPR_FPCSR);
  //unsigned int v_sr  = (unsigned int)or1k_mfspr(SPR_SR);
  //  printf("\r\n  Enable FPEE. FPSCR: %08X  SM: %1d\r\n", value,(v_sr&1));
  value = value | SPR_FPCSR_FPEE;
  // Write value back to FPCSR
  or1k_mtspr(SPR_FPCSR,value);
  */
  // NewLIB 2.4.0+
  unsigned int value = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_FPCSR_ADDR);
  //unsigned int v_sr  = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_SR_ADDR);
  //  printf("\r\n  Enable FPEE. FPSCR: %08X  SM: %1d\r\n", value,(v_sr&1));
  value = OR1K_SPR_SYS_FPCSR_FPEE_SET(value,0x1u);
  // Write value back to FPCSR
  or1k_mtspr(OR1K_SPR_SYS_FPCSR_ADDR,value);
}

void or1k_printf_fpcsr(void)
{
   //unsigned int value = (unsigned int)or1k_mfspr(SPR_FPCSR); // NewLIB 2.0.0
   unsigned int value = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_FPCSR_ADDR); // NewLIB 2.4.0+
   printf("  FPCSR: %08X\r\n", value);
}

// If FP-exception is enable we just count number of such exceptions.
static unsigned int fp_exceptions_cnt = 0;

// Interrupt handler for floating point exceptions
//  - increment FP-exceptions counter
//  - clean up SR[FPEE] bit (disable FP-exceptions to continue tests)
void float_except_handler(void)
{
  // Read the FPCSR
  unsigned int value;
  //unsigned int v_sr;

  ++fp_exceptions_cnt;

  // MAROCCHINO_TODO: all following code should be commented after
  //                  implementation FP-exceptions processing in
  //                  according with architectural manual

  /* NewLIB 2.0.0
  // Read the SPR
  value = (unsigned int)or1k_mfspr(SPR_FPCSR);

  //v_sr  = (unsigned int)or1k_mfspr(SPR_SR);
  //  printf("  FPU handler: Read FPCSR: %08X  SM=%1d\r\n", value,(v_sr&1));

  // drop FP-Exception-Enabled and write back to FPCSR
  value = value & (~SPR_FPCSR_FPEE);
  or1k_mtspr(SPR_FPCSR,value);
  */

  // NewLIB 2.4.0+
  // Read the SPR
  value = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_FPCSR_ADDR);

  //v_sr  = (unsigned int)or1k_mfspr(OR1K_SPR_SYS_SR_ADDR);
  //  printf("  FPU handler: Read FPCSR: %08X  SM=%1d\r\n", value,(v_sr&1));

  // drop FP-Exception-Enabled and write back to FPCSR
  value = OR1K_SPR_SYS_FPCSR_FPEE_SET(value, 0x0u);
  or1k_mtspr(OR1K_SPR_SYS_FPCSR_ADDR,value);
}

 // Illegal Instruction Handler
void illegal_insn_handler(void)
{
  printf(" Illegal instruction.\r\n");
  exit(1);
}

// Running this bare metal standalone for OR1K - hard set the configuration
int main( int argc, char **argv )
{
    flag functionArgument;
    uint8 functionCode;
    int8 operands, roundingPrecision, roundingMode;

    // Illegal Instruction Handler
    or1k_exception_handler_add(0x7, illegal_insn_handler);
    // Add exception handler for floating point exception
    or1k_exception_handler_add(0xd, float_except_handler);

    printf("Testfloat start.\r\n");


    fail_program_name = "testfloat";
    testCases_setLevel( 1 );
    trueName = "soft";
    testName = "syst";
    errorStop = FALSE;
    forever = FALSE;
    maxErrorCount = 1;
    trueFlagsPtr = &float_exception_flags;
    testFlagsFunctionPtr = syst_float_flags_read;
    tininessModeName = "after";
    float_detect_tininess = float_tininess_after_rounding;
    operands = 0;
    roundingPrecision = 0;
    roundingMode = 0;// ROUND_DOWN// - for only round down tests ;
                     //0 - for do all rounding modes
    // "all" setting:
    functionArgument = TRUE;
    functionCode = 0; // See testFunction.c for list.
    // 0  = all possible functions
    // --- single precision ---
    //  1 = int32_to_float32 :
    //  9 = float32_to_int32 :
    // 17 = float32_add :
    // 18 = float32_sub :
    // 19 = float32_mul :
    // 20 = float32_div :
    // 23 = float32_eq  :
    // 24 = float32_le  :
    // 25 = float32_lt  :
    // --- double precision ---
    //  6 = int64_to_float64
    // 31 = float64_to_int64
    // 37 = float64_add
    // 38 = float64_sub
    // 39 = float64_mul
    // 40 = float64_div
    // 43 = float64_eq
    // 44 = float64_le
    // 45 = float64_lt


    operands = 0;

    if ( ! functionArgument ) test_float_fail( "Function argument required" );
    //    (void) signal( SIGINT, catchSIGINT );
    //    (void) signal( SIGTERM, catchSIGINT );
    if ( functionCode ) {
        if ( forever ) {
            if ( ! roundingPrecision ) roundingPrecision = 80;
            if ( ! roundingMode ) roundingMode = ROUND_NEAREST_EVEN;
        }
        testFunction( functionCode, roundingPrecision, roundingMode );
    }
    else {

      for ( functionCode = 1;
                  functionCode < NUM_FUNCTIONS;
                  ++functionCode
                ) {
  //printf("trying function %d\r\n",functionCode);
                if ( functionExists[ functionCode ] ) {
                    testFunction(
                        functionCode, roundingPrecision, roundingMode );
                }
            }

    }

   #ifdef ENABLE_FP_EXCEPTION
    printf("Exceptions enabled: %u\r\n", fp_exceptions_cnt);
   #else
    printf("Exceptions disabled: %u\r\n", fp_exceptions_cnt);
   #endif

    exitWithStatus();

    // Should never reach here
    return 0;
} // main()
