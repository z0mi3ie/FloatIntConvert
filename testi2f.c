#include <stdio.h>
//#include "f2i.c"
//#include "i2f.c"

#include "prog1.c"

//int f2i(int);
int i2f(int);

int main(void)
{
  int  out, i, in;
  float f;

  in = 0x00000000;

  //int index;
  //for( index = 0; index < 5; index++ )
  //{
  while( in < 0xFFFFFFFF )
  {
    out = i2f(in);

    f = in;
    i = *(int *)&f;
    f = *(float *)&in;
    
    
    if( out == i )
    {
      //printf("\n +++ Good! " );
    }
    else
    {
      printf("\n --- Bad! " );
      printf("%08x (%e) -> %08x (my result is %08x)\n", in, f, i, out);
     break;
    }
  
    in++;
  }
    //}
  //}

  //    printf("completed: %08x (%e) -> %08x (my result is %08x)\n", in, f, i, out);
  return 0;
}
