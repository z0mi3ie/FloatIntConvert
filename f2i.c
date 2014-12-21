//Kyle Vickers
//Lab 3
//Cs520

/*Float to integer.*/
/* This function converts an integer passed to it, into a float based
 * on single precision IEEE format and returns it as an int */

#include <stdio.h>

static int SpecialCaseCheck( int, int );
static int Negate( int );

const int BIAS = 127;
const int SIGMASK = 0x007FFFFF;

//=========================================================
/* f2i(int)
 * this is the main converting function. This will return
 * the equivalent floating point value. 
 */
int f2i( int in )
{
  int exponent = 0;
  int sign = 0;
  int significant = 0;
  int hiddenone = 0;
  
  /* Get sign */  
  if( ( in & 0x80000000 ) == 0x80000000 )
  {
    sign = 1;
    in = in - 0x80000000; // remove sign from in
  }

  /* Get exponent */
  exponent = in >> 23;

  /* Get significant using 23 bit SIGMASK */
  significant = ( in & SIGMASK );

  /*Check the special cases given a unmodified exponent
   *  and an unmodified sig bits. */
  int sc = SpecialCaseCheck( exponent, significant );
  if( sc == 3 )
  {
    return 0;
  }
  else if( sc == 4 )
  {
    return 0;
  }

  /* put hidden 1 in for significant*/
  hiddenone = 0x00800000;
  significant = significant + hiddenone;
 
  /* Unbias exponent */
  exponent = exponent - BIAS;

  /* Move radix  */
  if( exponent <= 23 && exponent >= 0)
  {
    significant = significant >> ( 23 - exponent );  
  }
  else if( (exponent > 23)&&(exponent <31) )
  {
    significant = significant << ( exponent - 23 );
  }
  else if( exponent >= 31 )
  {
    /* Overflow */
    significant = 0x80000000;
  }
  else if( exponent < 0 )   
  {
    significant = 0;
  }

  /* Check sign bit and negate if needed */
  if( sign == 1 )
  {
    int negated = Negate(significant);
    significant = negated;
  }

  return significant;
}


//=========================================================
/* SpecialCaseCheck( int, int )
 * This method checks for the following special cases given 
 * a represent exponent and significant.
 * 1) Infinity
 * 2) NaN
 * 3) +- 0.0
 * 4) Denormalized
 */
static int SpecialCaseCheck( int e, int s )
{
  if( ( e && 0xFFFFFFFF ) == 0xFFFFFFFF && 
      ( s && 0x00000000 == 0x00000000 ) )
  {
    return 1;
  }
  if( ( ( e && 0xFFFFFFFF ) == 0xFFFFFFFF ) && 
      ( ( s && 0x00000000 ) != 0x00000000 ) )
  {
    return 2;
  }
  if( ( ( e || 0x00000000 ) == 0x00000000 ) && 
      ( ( s || 0x00000000 ) == 0x00000000 ) )
  {
    return 3;
  }
  if( ( ( e || 0x00000000 ) == 0x00000000 ) && 
      ( ( s || 0x00000000 ) != 0x00000000 ) )
  {
    return 4;
  }
  return 0;
}

//=========================================================
/* Negate( int )
 * Negates the input and returns the negated value */
static int Negate( int input )
{
  int output = ~input;
  output = output + 1;
  return output;
}

