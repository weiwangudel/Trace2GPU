for ( i0 = 0; i0 <= 4; i0++)
  Write
  142647808 + 8*i0
for ( i0 = 0; i0 <= 1023; i0++)
  Write
  142623104 + 8*i0
for ( i0 = 0; i0 <= 1023; i0++)
  Write
  142631328 + 8*i0
Write
142631328 + 16
for ( i0 = 0; i0 <= 1023; i0++)
  Write
  142639552 + 8*i0
for ( i0 = 0; i0 <= 4; i0++)
  for ( i1 = 0; i1 <= 31; i1++)
    Write
    142631328 + 8*i1
    Read
    142647808 + 8*i0
  for ( i1 = 0; i1 <= 991; i1++)
    Write
    142631328 + 256 + 8*i1
    Read
    142631328 + 256 + 8*i1
    Read
    142639552 + 256 + 8*i1
    Read
    142639552 + 8*i1
  for ( i1 = 0; i1 <= 31; i1++)
    for ( i2 = 0; i2 <= 30; i2++)
      Write
      142623104 + 8 + 256*i1 + 8*i2
      Read
      142623104 + 8 + 256*i1 + 8*i2
      Read
      142639552 + 8 + 256*i1 + 8*i2
      Read
      142639552 + 256*i1 + 8*i2
  for ( i1 = 0; i1 <= 30; i1++)
    for ( i2 = 0; i2 <= 30; i2++)
      Write
      142639552 + 256*i1 + 8*i2
      Read
      142639552 + 256*i1 + 8*i2
      Read
      142623104 + 8 + 256*i1 + 8*i2
      Read
      142623104 + 256*i1 + 8*i2
      Read
      142631328 + 256 + 256*i1 + 8*i2
      Read
      142631328 + 256*i1 + 8*i2
