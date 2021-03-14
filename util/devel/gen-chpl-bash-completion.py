#!/usr/bin/env python3

from __future__ import print_function
import sys
import os
import subprocess
import re

if "CHPL_HOME" in os.environ:
  pass
else:
  print("CHPL_HOME must be set to run this script", file=sys.stderr)
  exit(-1)

chpl_home=os.environ["CHPL_HOME"]

def gatherFlags(command):
    ret = [ ]
    lines = subprocess.check_output(command)
    for line in lines.splitlines():
        s=line.decode("utf-8").strip()
        if s.startswith("-"):
            # Split on spaces and on commas
            toks = re.split(r',| +', s)
            for t in toks:
                st = t.strip()
                # Ignore the strip characters
                if st == "" or st == ",":
                    continue
                # Stop working on this line for first word not starting with -
                if not st.startswith("-"):
                    break
                # OK, we think that it's an option
                if st.startswith("--[no-]"):
                    ret.append(st.replace("--[no-]", "--", 1))
                    ret.append(st.replace("--[no-]", "--no-", 1))
                else:
                    ret.append(st)

    return ret

devel=gatherFlags(["chpl", "--devel", "--help"])
nodevel=gatherFlags(["chpl", "--no-devel", "--help"])
devel.sort()
nodevel.sort()

# The separator for joining the arrays
sep=" \\\n"

template=chpl_home+"/util/devel/chpl-bash-completion-template"
with open(template) as f:
    for line in f:
        line=line.rstrip()
        line=line.replace("OPTS_DEVEL", sep.join(devel))
        line=line.replace("OPTS_NODEVEL", sep.join(nodevel))
        print(line)
