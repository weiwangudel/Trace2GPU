#pragma scop
for ( i0 = 0; i0 <= 9; i0++) 
    for ( i1 = 0; i1 <= 9; i1++)  
    {
        M [ 137859456 + 80*i0 + 8*i1 ] = 
        M [ 137859456 + 80*i0 + 8*i1 ] + 
        M [ 3219761584 ] ; 
        for ( i2 = 0; i2 <= 9; i2++) 
            M [ 137859456 + 80*i0 + 8*i1 ] = 
            M [ 137859456 + 80*i0 + 8*i1 ] + 
            M [ 3219761592 ] + 
            M [ 137860288 + 80*i0 + 8*i2 ] + 
            M [ 137861120 + 8*i1 + 80*i2 ] ; 
    }
#pragma endscop