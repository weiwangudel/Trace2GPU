#include <stdio.h>

#define DATA_TYPE double
#define DATA_PRINTF_MODIFIER "%0.2lf "
int main()
{
    double  A140013952 [ 8000 / 8];
    double  A140030016 [ 800 / 8];
    double  A140021984 [ 8000 / 8];

    int i0;
    int i1;
    int i2;
    int i3;

    #pragma scop
    for ( i0 = 0 ; i0 <= 9 ; i0++) 
    {
        for ( i1 = 0 ; i1 <= 9 ; i1++) 
        {
            for ( i2 = 0 ; i2 <= 9 ; i2++) 
            {
                 A140013952 [( 800*i0 + 80*i1 + 8*i2 ) / 8] 
                =((DATA_TYPE)(1*i0)             *(1*i1)             +(1*i2)             )/10
                ;
            }
        }
    }
    for ( i0 = 0 ; i0 <= 9 ; i0++) 
    {
        for ( i1 = 0 ; i1 <= 9 ; i1++) 
        {
             A140030016 [( 80*i0 + 8*i1 ) / 8] 
            =((DATA_TYPE)(1*i0)         *(1*i1)         )/10
            ;
        }
    }
    for ( i0 = 0 ; i0 <= 99 ; i0++) 
    {
        for ( i1 = 0 ; i1 <= 9 ; i1++) 
        {
             A140021984 [( 80*i0 + 8*i1 ) / 8] 
            =0
            ;
            for ( i2 = 0 ; i2 <= 9 ; i2++) 
            {
                 A140021984 [( 80*i0 + 8*i1 ) / 8] 
                =A140021984 [( 80*i0 + 8*i1 ) / 8]
                +A140013952 [( 80*i0 + 8*i2 ) / 8]
                *A140030016 [( 8*i1 + 80*i2 ) / 8]
                ;
            }
        }
        for ( i1 = 0 ; i1 <= 9 ; i1++) 
        {
             A140013952 [( 80*i0 + 8*i1 ) / 8] 
            =A140021984 [( 80*i0 + 8*i1 ) / 8]
            ;
        }
    }
    for ( i0 = 0 ; i0 <= 9 ; i0++) 
    {
        for ( i1 = 0 ; i1 <= 99 ; i1++) 
        {
    fprintf(stderr,DATA_PRINTF_MODIFIER,         A140013952 [( 800*i0 + 8*i1 ) / 8] 
            );
    if(         1*i0 
            %20
            ==0
            )fprintf(stderr,"\n");
        }
    }
    #pragma endscop

}
