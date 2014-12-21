/* Kyle Vickers
 * CS520 - Program 1 
 * IEEE Floating point conversions
 * 
 * i2f:
 * This function takes in a 32 bit integer as its argument
 * and converts the integer to a IEEE single precision
 * floating point number. It then returns this floating number.
 * 
 * f2i:
 * This function converts an integer passed to it, into a float based
 * on single precision IEEE format and returns it as an int 
 */


#include <stdio.h>

/* Forward declarations - Method descriptions found below */
static int shiftcount( int );
static int SpecialCaseCheck( int, int );
static int Negate( int );

/* Constant declarations */
const int BIAS = 127;
const int SIGMASK = 0x007FFFFF;

/* =============================================================
 * i2f( int )
 * This function converts a 32 bit integer to a float using
 * IEEE standards. There is a helper function called
 * from this for counting bit shifts to not clutter the code. 
 */
int i2f(int in)
{
 /* Save a copy of the input */
  int orig = in;

  /* Check to see if in is 0. If it is return 0 */
  if( orig == 0 )
  {
    return 0;
  }

  /* Save the sign to its own variable for later access */
  int sign = orig & 0x80000000;

  /* negate if original sign is negative */
  if(  sign == 0x80000000 )
  {
    orig = ~orig;
    orig = orig+1;
  }

  /* normalize, shift radix point to right */
  int numberofshifts = 0;
  numberofshifts = shiftcount( orig );
  unsigned int normalized = ( orig << numberofshifts );

  /* keep 23 significant bits */
  unsigned int sigmask = 0xFFFFFE00;
  unsigned int sig = ( normalized & sigmask ); 
  unsigned int shiftsig = ( sig >> 9 );

  /* get guard bits with a mask */
  unsigned int guardmask = 0x00000180;
  unsigned int guardbits = ( normalized & guardmask );
  
  /* get sticky bits with a mask */
  unsigned int stickymask = 0x0000007F;
  unsigned int stickybits = ( normalized & stickymask );
  
  /* get actual exponent */
  unsigned int actualexponent = ( 32 - numberofshifts ) + 127;


  /* check guard and sticky bits */
  /* if GB=11 add one to significant
   * if GB=10 then check
   *    1) if SB == 0 then
   *       Round to significant to even if odd
   *    2) if SB has any 1 then
   *       Add 1 to significant
   */   
  if( guardbits == 0x00000180 )
  {
    shiftsig = shiftsig + 1;
  }
  if( guardbits == 0x00000100 )
  {
    if( stickybits == 0x00000000 )
    {
      if( ( shiftsig & 0x00000001 ) == 1 )
      {
        shiftsig = shiftsig + 1;
      }
    }
    if( stickybits > 0 )
    {
      shiftsig = shiftsig + 1;
    }
  }

  /* put together  final answer */
  int out = 0x00000000;
  unsigned int shiftexponent = ( actualexponent << 23 );
  out = out + shiftexponent;
  out = out + shiftsig;

  if( sign == 0x80000000 )
  {
    out = out + 0x80000000;
  }

  return out;
}

//=========================================================
/* f2i(int)
 * this is the main converting function. This will return
 * the equivalent floating point value. 
 */
int f2i(int in)
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

/*~~~~~~~~~ Helper functions ~~~~~~~~~~~~~*/

/* =============================================================
 * shiftcount(int)
 * This function recursively shifts a bitwise function to the
 * right until a one is reached. Must have a 1 in order to
 * function properly.
 */
static int shiftcount( int  x )
{
  if( ( x & 0x80000000 ) == 0x80000000 )
  {
    return 1;
  }
  else
  {
    return 1 + shiftcount( x << 1 );
  }      
}

/* =============================================================
 * SpecialCaseCheck( int, int )
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

/* =============================================================
 * Negate( int )
 * Negates the input and returns the negated value */
static int Negate( int input )
{
  int output = ~input;
  output = output + 1;
  return output;
}

