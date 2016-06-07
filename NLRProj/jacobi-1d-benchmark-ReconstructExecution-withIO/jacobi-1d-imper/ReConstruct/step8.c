#include <stdio.h>

#define DATA_TYPE double
#define DATA_PRINTF_MODIFIER "%0.2lf "
int main()
{
    double  A150401408 [ 80000 / 8];
    double  A150481440 [ 80000 / 8];

    int i0;
    int i1;
    int i2;

    #pragma scop
    for ( i0 = 0 ; i0 <= 9999 ; i0++) 
    {
         A150401408 [( 8*i0 ) / 8] 
        =((DATA_TYPE)(1*i0)     +2
        )/10000
        ;
         A150481440 [( 8*i0 ) / 8] 
        =((DATA_TYPE)(1*i0)     +3
        )/10000
        ;
    }
    for ( i0 = 0 ; i0 <= 99 ; i0++) 
    {
        for ( i1 = 0 ; i1 <= 9997 ; i1++) 
        {
             A150481440 [( 8 + 8*i1 ) / 8] 
            =0.333330
            *(A150401408 [( 8*i1 ) / 8]
            +A150401408 [( 8 + 8*i1 ) / 8]
            +A150401408 [( 16 + 8*i1 ) / 8]
            );
        }
        for ( i1 = 0 ; i1 <= 9997 ; i1++) 
        {
             A150401408 [( 8 + 8*i1 ) / 8] 
            =A150481440 [( 8 + 8*i1 ) / 8]
            ;
        }
    }
    for ( i0 = 0 ; i0 <= 9999 ; i0++) 
    {
    fprintf(stderr,DATA_PRINTF_MODIFIER,     A150401408 [( 8*i0 ) / 8] 
        );
    if(     1*i0 
        %20
        ==0
        )fprintf(stderr,"\n");
    }
    #pragma endscop

}
