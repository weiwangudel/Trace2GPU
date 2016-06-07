# This version reads in reconstructed memory trace and execution trace 
# and output combined data

import sys                                                                       
import string
import re                                                                        
                                                                                 
#This program converts the NLR output to syntax-correct C SCoP program
#Input is intended to be the output of NLR

f = open(sys.argv[1], 'rU')               
lines = f.readlines()

exeTrace = open(sys.argv[2], 'rU')
exeLines = exeTrace.readlines()


curStatement = []
for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    if (string.find(line, "for") != -1):
	print line,
	continue
    if (string.find(line, "{") != -1):
	print line,
	continue
    if (string.find(line, "}") != -1):
	print line,
	continue
    if (string.find(line, "pragma") != -1):
	print line,
	continue
    # encounters Line, dump out MemoryTrace + ExecutionTrace here
    if (string.find(line, "Line") != -1):
	# retrieve information from the execution trace line	
	cur_indent = len(line) - len(line.lstrip())
	a = string.split(line)
  	for i in range (0, len(exeLines)):
    	    curExeLine = exeLines[i]
	    b = string.split(curExeLine, '@')
    	    if (long(b[0]) == long(a[1])):    # should use the value to match!
	  	#print exeLines[i]
		#print b[1]
		c = string.split(b[1], '0')
		#print c
		print c[0],
		print curStatement[0],	
  	        for i in range (1, len(curStatement)):
		   # prepare indentation
		   for j in range (0, cur_indent):
			print "", 
		   # judge if (mem-access) is needed
		   if ((string.find(curStatement[i], "+") != -1) or 
		      (string.find(curStatement[i], "-") != -1) or 
		      (string.find(curStatement[i], "*") != -1) or 
		      (string.find(curStatement[i], "/") != -1)) and (string.find(curStatement[i], "[") == -1):
	           	print c[i] + "(" + curStatement[i].strip()+")",
		   else:
	           	print c[i] + curStatement[i].strip()
		for j in range (0, cur_indent):
		      print "", 
		print c[len(c)-1],
	curStatement  = []
	continue
    else:
	curStatement.append(line)
