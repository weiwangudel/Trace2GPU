#!/bin/bash

 awk '{print NR"@ "$0}' $1  > $1-withLN.c
 awk '/Tns/ ' $1-withLN.c  > TnsOnly
 awk -F ";" '{print $1" ;"}' TnsOnly  > ET
 sed -i 's/TnsMem[Wr|C|Iter]*/Trace/g' ET 
 python mask.py  ET > masked
 sed -i 's/ //g' masked
 sed -i 's/\t//g' masked
 mv masked ET
 rm TnsOnly
