#!/usr/bin/env python2.7

import subprocess
import os
from collections import defaultdict
search_etc=subprocess.Popen("strace ./search /etc", stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
(out, err) = search_etc.communicate()
splut = err.split('\n')
#print(splut)
first_map = defaultdict(int)

for i in splut:
	if len(i)>0 and  i[0] != '-' and i[0].isalpha():
		first_map[i.split('(', 1)[0]]+=1
print("# search /etc")
for w in sorted(first_map, key=first_map.get, reverse=True):
	print(w + " " + str(first_map[w]))



search_etc=subprocess.Popen("strace ./search /etc -exec echo \{\} \;", stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
(out, err) = search_etc.communicate()
splut = err.split('\n')
#print(splut)
first_map = defaultdict(int)

for i in splut:
        if len(i)>0 and  i[0] != '-' and i[0].isalpha():
                first_map[i.split('(', 1)[0]]+=1
print("# search /etc -exec echo \{\} \;")
for w in sorted(first_map, key=first_map.get, reverse=True):
        print(w + " " + str(first_map[w]))

search_etc=subprocess.Popen("strace find /etc -exec echo \{\} \;", stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
(out, err) = search_etc.communicate()
splut = err.split('\n')
#print(splut)
first_map = defaultdict(int)

for i in splut:
        if len(i)>0 and  i[0] != '-' and i[0].isalpha():
                first_map[i.split('(', 1)[0]]+=1
print("# find /etc -exec echo \{\} \;")
for w in sorted(first_map, key=first_map.get, reverse=True):
        print(w + " " + str(first_map[w]))

