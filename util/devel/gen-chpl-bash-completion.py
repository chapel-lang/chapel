#!/usr/bin/env python

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
    with subprocess.Popen(command, stdout=subprocess.PIPE) as p:
        for line in p.stdout:
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

"""
sed -e "s/OPTS_DEVEL/$dev_opts/" -e "s/OPTS_NODEVEL/$nodev_opts/" $template
res = p.stdout.readlines()
if len(res) == 0: return False
return True
# Make sure chpl is in the path
# Make sure that chpl is on the path
source $CHPL_HOME/util/setchplenv.bash > /dev/null 2>&1

single_dev=`chpl --devel --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
            xargs echo`

positive_dev=`chpl --devel --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
              sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;\[no-\];;' |       \
              sed -e 's;-[A-Za-z],;   ;' | xargs echo`

negative_dev=`chpl --devel --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
              sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep '\[no-\]' |             \
              tr -d '[' | tr -d ']' | sed -e 's;-[A-Za-z],;   ;' | xargs echo`

single_nodev=`chpl --no-devel --help | grep -v '^[A-Za-z]' | grep '^.*--' |   \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
            xargs echo`

positive_nodev=`chpl --no-devel --help | grep -v '^[A-Za-z]' | grep '^.*--' | \
              sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;\[no-\];;' |       \
              sed -e 's;-[A-Za-z],;   ;' | xargs echo`

negative_nodev=`chpl --no-devel --help | grep -v '^[A-Za-z]' | grep '^.*--' | \
              sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep '\[no-\]' |             \
              tr -d '[' | tr -d ']' | sed -e 's;-[A-Za-z],;   ;' | xargs echo`

nodev_opts="$single_nodev $positive_nodev $negative_nodev"
dev_opts="$single_dev $positive_dev $negative_dev"

sed -e "s/OPTS_DEVEL/$dev_opts/" -e "s/OPTS_NODEVEL/$nodev_opts/" $template
"""
