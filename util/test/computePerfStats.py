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
	open_files()
	(keys, verify_keys, data_file) = read_key_file()
	valid_output = validate_output(verify_keys)
	if (valid_output):
		create_data_file()
		log_timeouts()
		find_keys()


def validate_output():
	# read output from file
	global test_output
	test_output = test_output_file.read().split("\n")
	test_output_file.close()
	errors_file.write("processed %s\n" % args.test_output_file)

	# check for valid output
	valid_output = True
	for key in verify_keys:
		# match verify keys
		m = re.match("(verify|reject):(?:(-?[1-9][0-9]*):)? ?(.+)", key)
		if not m:
			print "Error: invalid verify/reject line '%s'" % key
			exit(1)
		# set parts of match to variables
		type = m.group(0)
		num = m.group(1)
		regex = m.group(2)
		num_real = num;
		if num >= 1 : --num_real
		regex_real = regex
		is_reject = type == reject

		# depending on whether we're asked to verify or reject
		if not is_reject:
			search_msg = "Checking for"
			found_msg = "SUCCESS"
			not_found_msg = "FAILURE"
		else:
			search_msg = "Checking for absence of"
			found_msg = "FAILURE"
			not_found_msg = "SUCCESS"
		
		# MATCH
		if num: # if there's a line number
			print "%s /%s/ on line %s... " % (search_msg, regex, num)
			if re.match(regex_real, test_output[num_real]):
				valid_output &= !is_reject
				print found_msg
			else:
				valid_output &= is_reject
				print not_found_msg
		else: # no line number
			print "%s /%s/ on any line... " % (search_msg, regex)
			found = False
			for line in test_output:
				if re.match(regex_real, line):
					fount = True
					print found_msg
					valid_output &= !is_reject
					break
			if not found:
				valid_output &= is_reject
				print not_found_msg

		if not valid_output:
			print "Error: Invalid output found in %s" % args.test_output_file
	
	return valid_output


def read_key_file():
	verify_keys = []
	keys = []
	for line in keys_file:
		key = strip(line)
		if not key[0] == "#": # not a comment
			if "verify" in key or "reject" in key
				verify_keys.append(key)
			else:
				keys.append(key)
		else: # ignore comments unless they specify .dat
			comment = strip(key[1:])
			if comment[0:5] == "file:":
				data_file = "%s/%s" % (args.outputdir, comment.split()[1])
	keys_file.close()
	error_file.write("processed %s\n" % key_file_path)

	return (keys, verify_keys, data_file)


def open_files():
	global errors_file, keys_file, test_output_file
	if not os.access(error_file, os.W_OK):
		print "can't open %s for creating." % error_file
		exit()
	errors_file = open(error_file, "w")

	if not os.access(key_file_path, os.R_OK):
		print "can't open %s." % key_file_path
	keys_file = open(key_file_path, "r")

	if not os.access(args.test_output_file, os.R_OK):
		print "can't open %s." % args.test_output_file
	test_output_file = open(args.test_output_file, "r")


def setup():
	if not args.key_file_path:
		args.key_file_path = "%s.perfkeys" % args.testname;
	if not args.test_output_file:
		args.test_output_file = "%s.exec.out.tmp" % args.testname;
	if not args.perfdate:
		args.perfdate = time.srtftime("%m/%d/%y");
		print "Using default date %s" % args.perfdate;
	else:
		print "Using set date %s" % args.perfdate;
	
	global data_file, erorrs_file_path, fatal_errors;
	data_file = "%s/%s.dat" % (args.outputdir, args.testname)
	erorrs_file_path = "%s/%s.errors" % (args.outputdir, args.testname)
	fatal_errors = 0;

	if os.isfile(error_file):
		os.remove(error_file)
	

def parser_setup():
	parser = argparse.ArgumentParser(description="Compute performance"
			"statistics");
	parser.add_argument("testname");
	parser.add_argument("outputdir");
	parser.add_argument("key_file_path", nargs="?", default=False);
	paresr.add_argument("test_output_file", nargs="?", default=False);
	parser.add_argument("exec_time_out", nargs="?", default=False);
	parser.add_argument("perfdate", nargs="?", default=False);


if __name__ == "__main__":
	main()
