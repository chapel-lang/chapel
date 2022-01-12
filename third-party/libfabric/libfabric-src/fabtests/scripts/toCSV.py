#!/usr/bin/env python

import sys
import csv
from optparse import OptionParser

try:
	import yaml
except ImportError:
	print ("PyYAML library missing, try: yum install pyyaml")
	sys.exit(1)

def main(argv=None):
	"""Convert runfabtests.sh yaml output to CSV. If no argument is given
	   stdin is read, otherwise read from file.
	"""

	parser = OptionParser(description=main.__doc__, usage="usage: %prog [file]")
	(options, args) = parser.parse_args()

	if len(args) == 0:
		fd = sys.stdin
	else:
		fd = open(args[0], 'r')

	yi = yaml.load(fd.read())

	csv_fd = csv.writer(sys.stdout, delimiter=",", quotechar='"', quoting=csv.QUOTE_NONNUMERIC)
	csv_fd.writerow(["Test name", "Status"])
	
	for k,v in yi.items():
		csv_fd.writerow([k, v])

	return 0

if __name__ == "__main__":
	sys.exit(main(sys.argv))
