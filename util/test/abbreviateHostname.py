#!/usr/bin/env python3

import sys, re, os

def readAbbrevTable(filename):
  def readAbbrevTableImpl(f):
    entries = []
    for count, line in enumerate(f, start=1):
      line = line.strip()
      if len(line) == 0 or line[0] == '#':
        continue

      m = re.match(r'(.*)->(.*)', line)
      if not m:
        print("Error processing line %d in abbreviations file. Format "
          "should be 'regexp_matching_hostname -> abbreviation'" % count,
          file=sys.stderr)
        sys.exit(1)

      hostnameRegexp = m.group(1).strip()
      abbreviation = m.group(2).strip()

      try:
        re.compile(hostnameRegexp)
      except re.error:
        print("Invalid regexp pattern '%s' in file %s line %d" %
              (hostnameRegexp, os.path.abspath(filename), count),
              file=sys.stderr)
        sys.exit(1)

      entries.append((hostnameRegexp, abbreviation))

    return(entries)

  try:
    with open(sys.argv[2]) as f:
      return(readAbbrevTableImpl(f))
  except OSError:
    print("Error opening file: ", sys.argv[2], file=sys.stderr)
    sys.exit(1)


def applyAbbreviationTable(tbl, hostname):
  for (hostnameRegexp, abbreviation) in tbl:
    if re.match(hostnameRegexp, hostname):
      return abbreviation
  return(hostname)

if len(sys.argv) != 3:
  print("Invalid number of arguments", file=sys.stderr)
  print("Usage: %s hostname tableFilename" % sys.argv[0], file=sys.stderr)
  sys.exit(1)

hostname = sys.argv[1]
tableFilename = sys.argv[2]

abbrevTable = readAbbrevTable(tableFilename)
print(applyAbbreviationTable(abbrevTable, hostname))
