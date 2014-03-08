#This file read in indented loop structure
# output open and close parenthesised loop structure

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
last_indent = 0
is_first = 1 
# The following two are ds-init related
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
	if (last_indent == 0):     # brand new for
	    print line,
	    for i in range (0, indent):
		print "",
	    print "{"              # only print open paraenthesis
	else:
	   if (last_indent == indent): # for after statement
	     print line,
	     for i in range(0,indent):
		print "",
	     print "{"
	   elif (last_indent > indent):
	     while (last_indent > indent):
               	 last_indent -= 4  
	         for i in range(0, last_indent):
		    print "",
		 print "}"               # closing
	     print line,
	     for i in range (0, indent):
	       print "",
	     print "{"
	   else:
	     print line,
	     for i in range (0, indent):
	       print "",
	     print "{"
    else:
	if (last_indent >= indent):    # usually indent < last_indent
	     while (last_indent > indent):
               	 last_indent -= 4  
	         for i in range(0, last_indent):
		    print "",
		 print "}"               # closing
		
	print line,
	last_indent = indent       # set indentation       	
