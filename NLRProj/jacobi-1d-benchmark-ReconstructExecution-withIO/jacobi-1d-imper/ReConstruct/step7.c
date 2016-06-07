#pragma scop
for ( i0 = 0 ; i0 <= 9999 ; i0++) 
{
     A150401408 [( 8*i0 ) / 8] 
    =((DATA_TYPE)(1*i0)     +Constant2
    )/Constant10000
    ;
     A150481440 [( 8*i0 ) / 8] 
    =((DATA_TYPE)(1*i0)     +Constant3
    )/Constant10000
    ;
}
for ( i0 = 0 ; i0 <= 99 ; i0++) 
{
    for ( i1 = 0 ; i1 <= 9997 ; i1++) 
    {
         A150481440 [( 8 + 8*i1 ) / 8] 
        =Constant0.333330
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
    %Constant20
    ==Constant0
    )fprintf(stderr,"\n");
}
#pragma endscop
