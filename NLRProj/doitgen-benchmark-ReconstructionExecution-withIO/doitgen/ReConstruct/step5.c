#pragma scop
for ( i0 = 0 ; i0 <= 9 ; i0++) 
    for ( i1 = 0 ; i1 <= 9 ; i1++) 
        for ( i2 = 0 ; i2 <= 9 ; i2++) 
            A140013952 [( 800*i0 + 80*i1 + 8*i2 ) / 8] 
            1*i0 
            1*i1 
            1*i2 
            Constant10 
            Line34 
for ( i0 = 0 ; i0 <= 9 ; i0++) 
    for ( i1 = 0 ; i1 <= 9 ; i1++) 
        A140030016 [( 80*i0 + 8*i1 ) / 8] 
        1*i0 
        1*i1 
        Constant10 
        Line39 
for ( i0 = 0 ; i0 <= 99 ; i0++) 
    for ( i1 = 0 ; i1 <= 9 ; i1++) 
        A140021984 [( 80*i0 + 8*i1 ) / 8] 
        Constant0 
        Line79 
        for ( i2 = 0 ; i2 <= 9 ; i2++) 
            A140021984 [( 80*i0 + 8*i1 ) / 8] 
            A140021984 [( 80*i0 + 8*i1 ) / 8] 
            A140013952 [( 80*i0 + 8*i2 ) / 8] 
            A140030016 [( 8*i1 + 80*i2 ) / 8] 
            Line82 
    for ( i1 = 0 ; i1 <= 9 ; i1++) 
        A140013952 [( 80*i0 + 8*i1 ) / 8] 
        A140021984 [( 80*i0 + 8*i1 ) / 8] 
        Line87 
for ( i0 = 0 ; i0 <= 9 ; i0++) 
    for ( i1 = 0 ; i1 <= 99 ; i1++) 
        A140013952 [( 800*i0 + 8*i1 ) / 8] 
        Line56 
        1*i0 
        Constant20 
        Constant0 
        Line58 
#pragma endscop