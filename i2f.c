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
void testPrint( int, int, int, int, int, int, int, int, int, int, int, int  );
int shiftcount( int );


/* =============================================================
 * i2f( int )
 * This function converts a 32 bit integer to a float using
 * IEEE standards. There is a helper function called
 * from this for counting bit shifts to not clutter the code. 
 */
int i2f( int in )
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

/* =============================================================
 * shiftcount(int)
 * This function recursively shifts a bitwise function to the
 * right until a one is reached. Must have a 1 in order to
 * function properly.
 */
int shiftcount( int  x )
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
 * testPrint( ints )
 * This is my test function for viewing variables as they
 * change throughout my program
 */
void testPrint( int orig, int normalized, int sigmask, int sig,
                int guardmask, int guardbits, int stickymask,
                int stickybits, int actualexponent, int shiftexponent,
                int shiftsig, int out )
{
  printf("=======================================\n");
  printf("int orig: %08x\n", orig );
  printf("int normalized: %08x\n", normalized );
  printf("int sigmask: %08x\n", sigmask );
  printf("int sig: %08x\n", sig );
  printf("int guardmask: %08x\n", guardmask );
  printf("int guardbits: %08x\n", guardbits );
  printf("int stickymask: %08x\n", stickymask );
  printf("int stickybits: %08x\n", stickybits );
  printf("int actualexponent: %08x\n", actualexponent );
  printf("int shiftexponent: %08x\n", shiftexponent );
  printf("int shiftsig: %08x\n", shiftsig );
  printf("int out: %08x\n", out );
}
