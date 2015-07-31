#!/usr/bin/env python
text = open("time.txt", "r").read()
import re
def toMs(string):
    # e.g. "0m0.031s"
    matches = re.search("(\d+)m(\d+)\.(\d+)s", string )
    min = int(matches.group(1))
    sec = int(matches.group(2))
    ms  = int(matches.group(3))
    return (min*60 + sec)*1000 + ms
    

real = toMs( re.search("real\s+(\S+)", text).group(1) )
user = toMs( re.search("user\s+(\S+)", text).group(1) )
sys  = toMs( re.search("sys\s+(\S+)", text).group(1) )
print( "real {0}ms\nprogram {1}ms".format(real, user+sys) )
