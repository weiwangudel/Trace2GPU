for ( i0 = 0 ; i0 <= 4 ; i0++)
  Write
  158667264 + 8*i0
  Read
  1*i0
  Line36
for ( i0 = 0 ; i0 <= 31 ; i0++)
  for ( i1 = 0 ; i1 <= 31 ; i1++)
    Write
    158642560 + 256*i0 + 8*i1
    Read
    1*i0
    Read
    1*i1
    Read
    Constant1
    Read
    Constant32
    Line42
    Write
    158650784 + 256*i0 + 8*i1
    Read
    1*i0
    Read
    1*i1
    Read
    Constant2
    Read
    Constant32
    Line44
    Write
    158659008 + 256*i0 + 8*i1
    Read
    1*i0
    Read
    1*i1
    Read
    Constant3
    Read
    Constant32
    Line46
for ( i0 = 0 ; i0 <= 4 ; i0++)
  for ( i1 = 0 ; i1 <= 31 ; i1++)
    Write
    158650784 + 8*i1
    Read
    158667264 + 8*i0
    Line92
  for ( i1 = 0 ; i1 <= 991 ; i1++)
    Write
    158651040 + 8*i1
    Read
    158651040 + 8*i1
    Read
    Constant0.500000
    Read
    158659264 + 8*i1
    Read
    158659008 + 8*i1
    Line97
  for ( i1 = 0 ; i1 <= 31 ; i1++)
    for ( i2 = 0 ; i2 <= 30 ; i2++)
      Write
      158642568 + 256*i1 + 8*i2
      Read
      158642568 + 256*i1 + 8*i2
      Read
      Constant0.500000
      Read
      158659016 + 256*i1 + 8*i2
      Read
      158659008 + 256*i1 + 8*i2
      Line102
  for ( i1 = 0 ; i1 <= 30 ; i1++)
    for ( i2 = 0 ; i2 <= 30 ; i2++)
      Write
      158659008 + 256*i1 + 8*i2
      Read
      158659008 + 256*i1 + 8*i2
      Read
      Constant0.700000
      Read
      158642568 + 256*i1 + 8*i2
      Read
      158642560 + 256*i1 + 8*i2
      Read
      158651040 + 256*i1 + 8*i2
      Read
      158650784 + 256*i1 + 8*i2
      Line107
