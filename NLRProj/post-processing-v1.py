import sys                                                                       
import string
import re                                                                        
                                                                                 
#This program converts the NLR output to syntax-correct C SCoP program
#Input is intended to be the output of NLR

f = open(sys.argv[1], 'rU')               
lines = f.readlines()

fileStr = []
print "#pragma scop\n",
for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    if (string.find(line, "Write") != -1):
        fileStr += "//Write",
        fileStr += "\n",
    else:
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
                if (count == 1):
                    if (string.find(line, "for") == -1):
                        print "[", 
                print i,
                count += 1;
            # Add ] in the end
            if (count > 1 and string.find(line, "for") == -1):
                print "]",
            
            # Add "=" (write)
            if (string.find(lines[ln-1], "Write") != -1):
                print "=",  
            else:
                #Add "+" or ";"
                if (string.find(a[0], "0x") != -1 and ( (ln == len(lines) - 1) or (string.find(lines[ln+1], "for") != -1))):
                    print ";",
                else:
                    if (string.find(a[0], "0x") != -1):
                        print "+",
            print "\n",

print "#pragma endscop\n",
