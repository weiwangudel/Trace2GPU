#!/bin/bash

if [ $# -ne   2 ] 
then
  echo "usage: memory-trace-txt execution-trace-c"
  echo "for example:"
  echo "./reconstructFromTrace.sh gemver.memtrace.txt gemver.exectrace.c"
  exit
fi

./nlr -k 100 < $1 > step0.c
sed -i 's/;/ ;/g' step0.c
python gen-ds-init.py step0.c > step0-ds
python ds-reconst.py step0.c step0-ds > step1.c 
python post-processing-v4-nlr-output.py step1.c > step5.c
python replace-indentation.py  step5.c  > step6.c
./gen-ET.sh $2
sed -i 's/Line/Line /g' step6.c
python mt-add-et.py step6.c ET > step7.c 
python combine.py step7.c step0-ds > step8.c
sed -i 's/Constant//g' step8.c
echo "loop reconstructed, please see \"step8.c\" file located here!"
rm *with*
#rm step0-ds step1.c step5.c step6.c step7.c
