# COMPUTE PERFORMANCE STATISTTICS
# This is used by sub_test if start_test is called with the -performance flag.
# 
# For every performance run on a particular file, this script maintains a .dat
# by searchign athe output of the run for values of performance keys specified
# in .perfkeys. It then writes these valeus to the .dat for every run.

import argparse;
import os;
import time;

args = None

def main():
	parser = parser_setup()
	global args
	args = parser.parse_args()

	setup()


def open_files():
	global errors, keys
	if not os.access(error_file, os.W_OK):
		print "can't open %s for creating." % error_file
		exit()
	errors = open(error_file, "w")

	if not os.access(key_file, os.R_OK):
		print "can't open %s." % key_file
	keys = open(key_file, "r")


def setup():
	if not args.key_file:
		args.key_file = "%s.perfkeys" % args.testname;
	if not args.test_output_file:
		args.test_output_file = "%s.exec.out.tmp" % args.testname;
	if not args.perfdate:
		args.perfdate = time.srtftime("%m/%d/%y");
		print "Using default date %s" % args.perfdate;
	else:
		print "Using set date %s" % args.perfdate;
	
	global data_file, errors_file, fatal_errors;
	data_file = "%s/%s.dat" % (args.outputdir, args.testname)
	errors_file = "%s/%s.errors" % (args.outputdir, args.testname)
	fatal_errors = 0;

	if os.isfile(error_file):
		os.remove(error_file)


def parser_setup():
	parser = argparse.ArgumentParser(description="Compute performance"
			"statistics");
	parser.add_argument("testname");
	parser.add_argument("outputdir");
	parser.add_argument("key_file", nargs="?", default=False);
	paresr.add_argument("test_output_file", nargs="?", default=False);
	parser.add_argument("exec_time_out", nargs="?", default=False);
	parser.add_argument("perfdate", nargs="?", default=False);


if __name__ == "__main__":
	main()
