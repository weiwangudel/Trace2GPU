for ( i0 = 0 ; i0 <= 9 ; i0++)
  for ( i1 = 0 ; i1 <= 9 ; i1++)
    for ( i2 = 0 ; i2 <= 9 ; i2++)
      Write
      140013952 + 800*i0 + 80*i1 + 8*i2
      Read
      1*i0
      Read
      1*i1
      Read
      1*i2
      Read
      Constant10
      Line34
for ( i0 = 0 ; i0 <= 9 ; i0++)
  for ( i1 = 0 ; i1 <= 9 ; i1++)
    Write
    140030016 + 80*i0 + 8*i1
    Read
    1*i0
    Read
    1*i1
    Read
    Constant10
    Line39
for ( i0 = 0 ; i0 <= 99 ; i0++)
  for ( i1 = 0 ; i1 <= 9 ; i1++)
    Write
    140021984 + 80*i0 + 8*i1
    Read
    Constant0
    Line79
    for ( i2 = 0 ; i2 <= 9 ; i2++)
      Write
      140021984 + 80*i0 + 8*i1
      Read
      140021984 + 80*i0 + 8*i1
      Read
      140013952 + 80*i0 + 8*i2
      Read
      140030016 + 8*i1 + 80*i2
      Line82
  for ( i1 = 0 ; i1 <= 9 ; i1++)
    Write
    140013952 + 80*i0 + 8*i1
    Read
    140021984 + 80*i0 + 8*i1
    Line87
for ( i0 = 0 ; i0 <= 9 ; i0++)
  for ( i1 = 0 ; i1 <= 99 ; i1++)
    Read
    140013952 + 800*i0 + 8*i1
    Line56
    Read
    1*i0
    Read
    Constant20
    Read
    Constant0
    Line58
