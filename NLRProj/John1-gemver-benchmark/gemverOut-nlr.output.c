for i0 = 0 to 0x63
  for i1 = 0 to 0x63
    val Write
      , 0x601440 + 800*i0 + 8*i1
      , Read
      , 0x601440 + 800*i0 + 8*i1
      , Read
      , 0x628860 + 8*i0
      , Read
      , 0x6291c0 + 8*i1
      , Read
      , 0x628b80 + 8*i0
      , Read
      , 0x628ea0 + 8*i1
for i0 = 0 to 0x63
  for i1 = 0 to 0x63
    val Write
      , 0x628540 + 8*i0
      , Read
      , 0x628540 + 8*i0
      , Read
      , 0x601430
      , Read
      , 0x601440 + 8*i0 + 800*i1
      , Read
      , 0x629800 + 8*i1
for i0 = 0 to 0x63
  val Write
    , 0x628540 + 8*i0
    , Read
    , 0x628540 + 8*i0
    , Read
    , 0x629b20 + 8*i0
for i0 = 0 to 0x63
  for i1 = 0 to 0x63
    val Write
      , 0x6294e0 + 8*i0
      , Read
      , 0x6294e0 + 8*i0
      , Read
      , 0x601428
      , Read
      , 0x601440 + 800*i0 + 8*i1
      , Read
      , 0x628540 + 8*i1
