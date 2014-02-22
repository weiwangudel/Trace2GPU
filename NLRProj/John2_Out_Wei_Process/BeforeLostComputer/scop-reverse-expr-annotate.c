#pragma scop
for ( i0 = 0; i0 <= 11; i0++) 
    for ( i1 = 0; i1 <= 2; i1++) 
        M[ 3733676 ] + 
        M[ 3734052 + 8*i0 + 96*i1 ] = 
//=============Above: LowpassFilter line 12@John2.cpp ================= 
for ( i0 = 0; i0 <= 11; i0++) 
    M[ 3734032 ] + 
    M[ 3734032 ] + 
    M[ 3734052 + 8*i0 ] = 
    M[ 3734032 ] + 
    M[ 3734052 + 8*i0 ] + 
    M[ 3734032 ] + 
    M[ 3734148 + 8*i0 ] = 
    M[ 3734032 ] + 
    M[ 3734148 + 8*i0 ] + 
    M[ 3734032 ] + 
    M[ 3734244 + 8*i0 ] = 
//=============Above: John2Filter lines 15-20@John2.cpp================ 
for ( i0 = 0; i0 <= 2; i0++) 
    M[ 3733688 ] + 
    M[ 3733692 ] + 
    M[ 3733696 ] + 
    M[ 3733636 ] + 
    M[ 3734052 ] = 
    M[ 3733696 ] + 
    M[ 3733636 ] + 
    M[ 3734148 ] = 
    M[ 3733696 ] + 
    M[ 3733636 ] + 
    M[ 3734244 ] = 
// not ideal since it reconstructed the following boundary
    for ( i1 = 0; i1 <= 2 + 4*i0; i1++) 
        M[ 3733636 ] + 
        M[ 3734052 + 8*i1 ] + 
        M[ 3733636 ] + 
        M[ 3734148 + 8*i1 ] + 
        M[ 3733636 ] + 
        M[ 3734244 + 8*i1 ] + 
        M[ 3733660 ] = 
        for ( i2 = 0; i2 <= 2; i2++) 
            M[ 3733696 ] + 
            M[ 3733636 ] + 
            M[ 3734060 + 8*i1 + 96*i2 ] = 
    M[ 3733636 ] + 
    M[ 3734076 + 32*i0 ] + 
    M[ 3733636 ] + 
    M[ 3734172 + 32*i0 ] + 
    M[ 3733636 ] + 
    M[ 3734268 + 32*i0 ] + 
    M[ 3733660 ] = 
//=================== Above: blind reconstruction of the following ==============
// Lines 25-28@John2.cpp
//    for (int i=0; i<DATA_LEN; i+=blockLen)      
//    {                                          
//        filter.SetValueAt(i, blockLen,3);     
//    } 
//=======================================================================  
for ( i0 = 0; i0 <= 4; i0++) 
    M[ 3733688 ] + 
    M[ 3733692 ] + 
    M[ 3733696 ] + 
    M[ 3733636 ] + 
    M[ 3734052 ] = 
    M[ 3733696 ] + 
    M[ 3733636 ] + 
    M[ 3734148 ] = 
    M[ 3733696 ] + 
    M[ 3733636 ] + 
    M[ 3734244 ] = 
    for ( i1 = 0; i1 <= 2 + 2*i0; i1++) 
        M[ 3733636 ] + 
        M[ 3734052 + 8*i1 ] + 
        M[ 3733636 ] + 
        M[ 3734148 + 8*i1 ] + 
        M[ 3733636 ] + 
        M[ 3734244 + 8*i1 ] + 
        M[ 3733660 ] = 
        for ( i2 = 0; i2 <= 2; i2++) 
            M[ 3733696 ] + 
            M[ 3733636 ] + 
            M[ 3734060 + 8*i1 + 96*i2 ] = 
    M[ 3733636 ] + 
    M[ 3734076 + 16*i0 ] + 
    M[ 3733636 ] + 
    M[ 3734172 + 16*i0 ] + 
    M[ 3733636 ] + 
    M[ 3734268 + 16*i0 ] + 
    M[ 3733660 ] = 
//============The above: reconstructs Lines 31-34@John2.cpp ============
//    for (int i=0; i<DATA_LEN-blockLen/2; i+=blockLen/2)
//    {                                                  
//        filter.SetValueAt(i, blockLen,5);              
//    }
//=============================================================
for ( i0 = 0; i0 <= 11; i0++) 
    for ( i1 = 0; i1 <= 2; i1++) 
        M[ 3733452 ] + 
        M[ 2453520 + 8*i0 + 96*i1 ] = 
//============= above:  CLowpassFilter* pFilter = new CHighpassFilter(); =======
for ( i0 = 0; i0 <= 4; i0++) 
    for ( i1 = 0; i1 <= 2; i1++) 
        M[ 3733696 ] + 
        M[ 3733688 ] + 
        M[ 2453520 + 16*i0 + 96*i1 ] = 
//===============above: lines 39:42@John2.cpp  SetValueAt in HighpassFilter file ==============
for ( i0 = 0; i0 <= 2; i0++) 
    M[ 3733696 ] + 
    M[ 3733688 ] + 
    M[ 2453520 + 96*i0 ] = 
//===============above: non-parallel  i=0 case Line49 @John2.cpp =========================
for ( i0 = 0; i0 <= 3; i0++) 
    for ( i1 = 0; i1 <= 2; i1++) 
        M[ 3733696 ] + 
        M[ 3733688 ] + 
        M[ 2453536 + 16*i0 + 96*i1 ] = 
    M[ 3733960 ] + 
    M[ 2453528 + 16*i0 ] + 
    M[ 3733960 ] + 
    M[ 2453536 + 16*i0 ] + 
    M[ 3733960 ] + 
    M[ 2453528 + 16*i0 ] = 
//=================above: non-parallel in John2.cpp i>0 case, last loop of John2.cpp=======
#pragma endscop
