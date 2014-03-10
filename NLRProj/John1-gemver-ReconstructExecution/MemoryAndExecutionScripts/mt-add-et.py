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
    	    if (string.find(curExeLine, a[1]) != -1):
	  	#print exeLines[i]
		b = string.split(curExeLine, '@')
		#print b[1]
		c = string.split(b[1], '0')
		#print c
		print curStatement[0],	
  	        for i in range (1, len(curStatement)):
		   # prepare indentation
		   for j in range (0, cur_indent):
			print "", 
	           print c[i] + curStatement[i],
		for j in range (0, cur_indent):
		      print "", 
		print c[len(c)-1],
	curStatement  = []
	continue
    else:
	curStatement.append(line)
