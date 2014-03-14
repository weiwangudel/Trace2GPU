# This version is to use different array names rather than the global M.
# For the version that uses global M, use version 3. This is version 4.
# dedicated to use A01234 to represent (the constant address).
# date: 02/21/2014 12:10AM

import sys                                                                       
import string
import re                                                                        
                                                                                 
#This program converts the NLR output to syntax-correct C SCoP program
#Input is intended to be the output of NLR

f = open(sys.argv[1], 'rU')               
lines = f.readlines()

curStatement = []
print "#pragma scop\n",
for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    if (string.find(line, "Write") != -1):
	continue
    else:
        if (string.find(line, "Read") != -1):
            continue
        # Need to remember the number of identation
        indent = len(line) - len(line.lstrip()) 
        a = string.split(line)
        # Ignore Blank Line
        if (len(a) > 0):  
            # Print Indentation
            for i in range(0, indent):
                print " ",
            # Add array [ ] symbols
            count = 0
            for i in a:
                # Add [
                if (count == 0):
                    if (string.find(line, "for") == -1):
		      if (str.isdigit(i)):
                        print "A"+i, 
			count += 1;
			continue
		if (count == 1):
			if (string.find(line, "for") == -1):
		            print "[(",
			    if (string.find(i, "+") != -1):
			        count += 1;
				continue
                print i,
                count += 1;
            if (count > 1):
		# Add ] in the end
            	if (string.find(line, "for") == -1):
               	    print ") / 8]",
    print "\n",
print "#pragma endscop\n",
