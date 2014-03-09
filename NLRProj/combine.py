#This python code take in array.info and scop and 
#try to build a compilable program

import sys                                                                       
import string
import re      


print "#include <stdio.h>"
print ""
print "int main()"
print "{"


# read in the ds structure
# store the base address and compute the ending address
f_ds = open(sys.argv[2], 'r')
arrays = f_ds.readlines()
for cur in range(0, len(arrays)):
  temp = string.split(arrays[cur])
  print "    double", 
  print " A"+temp[0],
  print "[",
  print temp[1],
  print "/ 8];"

f_ds.close()

print ""
#for i in range(len(bases)):
#  print bases[i]
#  print tops[i]

f = open(sys.argv[1], 'rU')               
lines = f.readlines()

max = 0;
for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    indent = len(line) - len(line.lstrip())
    if (max < indent):
	max = indent
count = 0
while (max > 0):
    print "    int i"+str(count)+";"
    count += 1
    max -= 4
print ""

for ln in range(0, len(lines)):  ## iterates over the lines of the file
    line = lines[ln]
    print "    "+line,

print ""
print "}"
