#This file read in step0.c file and generate the data structure info
#The info is output/organized as follows:
# array1 size1 (in bytes)
# array2 size2 (in bytes)


import sys                                                                       
import string
import re                                                                        
                                                                                 

def belongs_to(address,bases,tops):
  flag = 0    
  for i in range(len(bases)):
    if ((long(address) >= long(bases[i])) and 
	(long(address) < long(bases[i] + tops[i]))):
   #   print bases[i],
   #   print "+",
   #   print (long(address) - bases[i]),
	return True
  if (not flag):
	return False


f = open(sys.argv[1], 'rU')               
lines = f.readlines()

fileStr = []
loop_upper = []
cur_indent = 0
# The following two are ds-init related
bases = []
tops = []
for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    if (string.find(line, "Write") != -1):
	continue
    if (string.find(line, "Read") != -1):
	continue
        # Need to remember the number of identation
    indent = len(line) - len(line.lstrip()) 
    a = string.split(line)
    if (string.find(line, "for") != -1):
	if (indent == 0):  
	    loop_upper = []
	    loop_upper.append(long(a[8])+1)
	    cur_indent = indent
	elif (indent > cur_indent):
 	    loop_upper.append(long(a[8])+1)
	    cur_indent = indent
	elif (indent == cur_indent):
	    loop_upper.pop()
	    loop_upper.append(long(a[8])+1)
	else:
	    while (cur_indent > indent):
		loop_upper.pop()
		cur_indent -= 2
	    loop_upper.pop()
	    loop_upper.append(long(a[8])+1)
    else:   # statements that begin with number
	# still look at indent
	# not in any for loop
	if (indent == 0):
 	  if (not belongs_to(long(a[0]), bases, tops)):
	    base = long(a[0])
	    top = long(8)
	    bases.append(base)
	    tops.append(top)   	
	# in a for loop	
	else:
 	  if (not belongs_to(long(a[0]), bases, tops)):
	    base = long(a[0])
	    top = long(8) 
	    for i in range(0, len(loop_upper)):
	    	top *= long(loop_upper[i])
	    bases.append(base)
	    tops.append(top)   	
for i in range(0, len(bases)):
  print bases[i],
  print tops[i]
