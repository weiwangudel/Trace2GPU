#pragma scop
for ( i0 = 0; i0 <= 99; i0++) 
    A160661888 [( 8*i0 ) / 8] = 
    A3216936856 + 
    A3216936860 ; 
for ( i0 = 0; i0 <= 99; i0++) 
    A160662720 [( 8*i0 ) / 8] = 
    A3216936856 + 
    A3216936860 ; 
for ( i0 = 0; i0 <= 99; i0++) 
    A160663552 [( 8*i0 ) / 8] = 
    A3216936856 + 
    A3216936860 ; 
for ( i0 = 0; i0 <= 9; i0++) 
    for ( i1 = 0; i1 <= 9; i1++) 
        A160661888 [( 80*i0 + 8*i1 ) / 8] = 
        A160661888 [( 80*i0 + 8*i1 ) / 8] + 
        A3216936832 ; 
        for ( i2 = 0; i2 <= 9; i2++) 
            A160661888 [( 80*i0 + 8*i1 ) / 8] = 
            A160661888 [( 80*i0 + 8*i1 ) / 8] + 
            A3216936840 + 
            A160662720 [( 80*i0 + 8*i2 ) / 8] + 
            A160663552 [( 8*i1 + 80*i2 ) / 8] ; 
#pragma endscop
