# This script masks TnsMem[Wr|C|Iter]() into 0s.

import sys                                                                       
import string
import re                                                                        
                                                                                 
#This program converts the NLR output to syntax-correct C SCoP program
#Input is intended to be the output of NLR

f = open(sys.argv[1], 'rU')               
lines = f.readlines()

cur_print = []
for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    indent = len(line) - len(line.lstrip()) 
    a = line.split("Trace")
    print a[0],
    for i in range(1, len(a)):
	# begin remove the (****) associated with Trace, replace them with 0 
        flag = 0;
	count = 0;
	for char in a[i]:
	  if (flag == 0 and (char == '(')):
	    count+=1
	  elif (flag == 0 and (char == ')')):
	    count-=1
	    if (count == 0):
	      flag = 1
	      print "0",
	  else:
	      if (flag > 0):
		print str(char),
