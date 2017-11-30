#!/usr/bin/python
import sys
import re
import os

cmdargs = str(sys.argv[1])
fh = open( cmdargs , "r")
fh_out = open("./array32.bitmap","a")
output_header = "const uint32_t RODATA_ATTR"
file_name = (os.path.splitext(os.path.basename(cmdargs))[0])
lines = fh.readlines()
array = []
array2 = []
for line in lines:
	array.append(line.split(','))
'''print(array)'''
for line in array:
	for memb in line:
		m = re.search(' 0x([0-9a-fA-F]{2})',memb)
		if m:
			try:
				array2.append(m.group(1))
			except AttributeError:
				continue
array2len = len(array2)
'''print(array2)'''
array32 = []
i = 0
while i < array2len - 3:
	array32.append("0x" + array2[i] + array2[i+1] + array2[i+2] + array2[i+3] )
	i += 4
'''print(array32)'''
b = 0
fh_out.write("{0} {1}{2}\n\t\t".format(output_header,file_name,"[] = {"))
while b < (len(array32)-1):
	if( (b + 1) % 3 == 0):
		fh_out.write("%s,\n\t\t" % (array32[b]))
	else:
		fh_out.write("%s, " % (array32[b]))
	b += 1
fh_out.write("{0}{1}\n".format(array32[b], "};"))
fh.close()
fh_out.close()
	
