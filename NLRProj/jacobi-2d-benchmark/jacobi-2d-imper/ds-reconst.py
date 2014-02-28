# This python code is to take in nlrOutput and perform the first processing.
# It looks at all memory accesses and make the memory accesses that happen
# in the same array begin with the same constant (the base address).
# It also reads in the "ds" file for data structure (array) look-up.
# Mainly developed for GPU code generation for stencil programs.

import sys                                                                       
import string
import re      

def belongs_to(address,bases,tops):
  flag = 0    
  for i in range(len(bases)):
    if ((long(address) > bases[i]) and 
	(long(address) < tops[i])):
      print bases[i],
      print "+",
      print (long(address) - bases[i]),
      flag = 1
      break 
  if (not flag):
	print address,
                                                          

# read in the ds structure
# store the base address and compute the ending address
f_ds = open(sys.argv[2], 'r')
arrays = f_ds.readlines()
bases = []
tops = []
for cur in range(0, len(arrays)):
  temp = string.split(arrays[cur])
  base = long(temp[0])
  top = long(base) + long(temp[1])
  bases.append(base)
  tops.append(top)
f_ds.close()

#for i in range(len(bases)):
#  print bases[i]
#  print tops[i]

f = open(sys.argv[1], 'rU')               
lines = f.readlines()

fileStr = []
for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    a = string.split(line)
    if (len(a) <= 0):    ## ignore blank line 
      continue
    if ( not a[0].isdigit() ):    ## if not begin with number, print the line
	print line,
    else:      ## begin with number: processing the first number
        # first, print the indentation
	indent = len(line) - len(line.lstrip())
	for i in range(0, indent):
	  print "",
	belongs_to(a[0], bases, tops)	
	for i in range(1, len(a)):
	  print a[i],
	print 
