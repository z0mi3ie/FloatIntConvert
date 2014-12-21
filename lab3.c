#include <stdio.h>
#include "prog1.c"

int f2i(int);

int main(void)
{
  int in;
  float f;
  int i;
  int f2i_saved;

 // in = 0x4FAAAAAA;
 // in = 0x47AAAAAA; 
 // in = 0xbf85AAAA;
 // in = 0x8059FD12;  
 // in = 0x3F800000;
 // in = 0xC74562AE; 
 // in = 0x80000000;
 in = 0xF0000000;

  //long int index = 0;
  //for( index = 0; index <= 0xFFFFFFFF ; index+=1000 )
  //while( in <= 0x4FFFFFFF )
  //while( in <= 0x3F800002 )
  //{
  while( in < 0xFFFFFFFF )
  {
   f = *(float *)&in;
   i = f;
   f2i_saved = f2i(in);
   
   /*if(index % 100000000 == 0)
   {
     printf( "index: %08d in: %08x\n", index, in );
   }*/

   if( i == f2i_saved)
   {
    //if( index % 1000000000 == 0 )
      // printf("Good!!! %08x (%e) --> %08x (my result is %08x)\n", in, f, i, f2i_saved);
   }
   else
   {
  //   if( index % 1000000 == 0 )
      printf("Bad!!! %08x (%e) --> %08x (my result is %08x)\n", in, f, i, f2i_saved);
   }

   in++;
   //index++;
  }
 

  printf("Last one tested:  %08x (%e) --> %08x (my result is %08x)\n", in, f, i, f2i_saved);
  printf( "\nCleared.\n");
  return 0;
  
}
