
/*
===============================================================================

This GNU assembler source file is part of TestFloat, Release 2a, a package
of programs for testing the correctness of floating-point arithmetic
complying to the IEC/IEEE Standard for Floating-Point.

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

  .text

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_int32_to_float32
syst_int32_to_float32:
  lf.itof.s r11,r3
  l.jr r9
  l.nop


/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_to_int32_round_to_zero
syst_float32_to_int32_round_to_zero:
  lf.ftoi.s r11,r3
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_to_int32
syst_float32_to_int32:
  lf.ftoi.s r11,r3
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_round_to_int
syst_float32_round_to_int:
  lf.ftoi.s r11,r3
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_add
syst_float32_add:
  lf.add.s r11,r3,r4
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_sub
syst_float32_sub:
  lf.sub.s r11,r3,r4
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_mul
syst_float32_mul:
  lf.mul.s r11,r3,r4
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_div
syst_float32_div:
  lf.div.s r11,r3,r4
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_eq
syst_float32_eq:
  lf.sfeq.s r3,r4
  l.bnf f32eq_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f32eq_exit:
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_le
syst_float32_le:
  lf.sfle.s r3,r4
  l.bnf  f32le_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f32le_exit:
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float32_lt
syst_float32_lt:
  lf.sflt.s r3,r4
  l.bnf  f32lt_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f32lt_exit:
  l.jr r9
  l.nop




/*
-------------------------------------------------------------------------------
Long-Long to Double
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_int64_to_float64
syst_int64_to_float64:
  /* GCC5
  lf.itof.d r11,r3
  l.jr r9
  l.nop
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r19,r4,r4
  lf.itof.d r17,r17
  l.or r11,r17,r17
  l.jr r9
  l.or r12,r19,r19

/*
-------------------------------------------------------------------------------
Double to Long-Long.
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_to_int64
syst_float64_to_int64:
  /* GCC5
  lf.ftoi.d r11,r3
  l.jr r9
  l.nop
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r19,r4,r4
  lf.ftoi.d r17,r17
  l.or r11,r17,r17
  l.jr r9
  l.or r12,r19,r19


/*
-------------------------------------------------------------------------------
Double to Long-Long with rounding to zero. Actually same to previous
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_to_int64_round_to_zero
syst_float64_to_int64_round_to_zero:
  /* GCC5
  lf.ftoi.d r11,r3
  l.jr r9
  l.nop
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r19,r4,r4
  lf.ftoi.d r17,r17
  l.or r11,r17,r17
  l.jr r9
  l.or r12,r19,r19

/*
-------------------------------------------------------------------------------
Double addition
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_add
syst_float64_add:
  /* GCC5
  lf.add.d r11,r3,r5
  l.jr r9
  l.nop 0x0
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r21,r5,r5
  l.or r19,r4,r4
  l.or r23,r6,r6
  lf.add.d r17,r17,r21
  l.or r11,r17,r17
  l.jr r9
  l.or r12,r19,r19

/*
-------------------------------------------------------------------------------
Double substruction
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_sub
syst_float64_sub:
  /* GCC5
  lf.sub.d r11,r3,r5
  l.jr r9
  l.nop 0x0
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r21,r5,r5
  l.or r19,r4,r4
  l.or r23,r6,r6
  lf.sub.d r17,r17,r21
  l.or r11,r17,r17
  l.jr r9
  l.or r12,r19,r19

/*
-------------------------------------------------------------------------------
Duoble multiplication
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_mul
syst_float64_mul:
  /* GCC5
  lf.mul.d r11,r3,r5
  l.jr r9
  l.nop 0x0
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r21,r5,r5
  l.or r19,r4,r4
  l.or r23,r6,r6
  lf.mul.d r17,r17,r21
  l.or r11,r17,r17
  l.jr r9
  l.or r12,r19,r19

/*
-------------------------------------------------------------------------------
Double division
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_div
syst_float64_div:
  /* GCC5
  lf.div.d r11,r3,r5
  l.jr r9
  l.nop 0x0
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r21,r5,r5
  l.or r19,r4,r4
  l.or r23,r6,r6
  lf.div.d r17,r17,r21
  l.or r11,r17,r17
  l.jr r9
  l.or r12,r19,r19

/*
-------------------------------------------------------------------------------
Double A == Double B
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_eq
syst_float64_eq:
  /* GCC5
  lf.sfeq.d r3,r5
  l.bnf f64eq_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f64eq_exit:
  l.jr r9
  l.nop
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r21,r5,r5
  l.or r19,r4,r4
  l.or r23,r6,r6
  lf.sfeq.d r17,r21
  l.bnf f64eq_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f64eq_exit:
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
Double A <= Double B
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_le
syst_float64_le:
  /* GCC5
  lf.sfle.d r3,r5
  l.bnf  f64le_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f64le_exit:
  l.jr r9
  l.nop
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r21,r5,r5
  l.or r19,r4,r4
  l.or r23,r6,r6
  lf.sfle.d r17,r21
  l.bnf f64le_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f64le_exit:
  l.jr r9
  l.nop

/*
-------------------------------------------------------------------------------
Double A < Double B
-------------------------------------------------------------------------------
*/
  .align 4
  .global syst_float64_lt
syst_float64_lt:
  /* GCC5
  lf.sflt.d r3,r5
  l.bnf  f64lt_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f64lt_exit:
  l.jr r9
  l.nop
  */
  /* GCC9 */
  l.or r17,r3,r3
  l.or r21,r5,r5
  l.or r19,r4,r4
  l.or r23,r6,r6
  lf.sflt.d r17,r21
  l.bnf f64lt_exit
  l.addi r11,r0,0
  l.addi r11,r0,1
f64lt_exit:
  l.jr r9
  l.nop
