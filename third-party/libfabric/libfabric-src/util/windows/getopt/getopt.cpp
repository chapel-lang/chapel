/*
* Copyright (c) 2013-2016 Intel Corporation.  All rights reserved.
*
* This software is available to you under the BSD license below:
*
*     Redistribution and use in source and binary forms, with or
*     without modification, are permitted provided that the following
*     conditions are met:
*
*      - Redistributions of source code must retain the above
*        copyright notice, this list of conditions and the following
*        disclaimer.
*
*      - Redistributions in binary form must reproduce the above
*        copyright notice, this list of conditions and the following
*        disclaimer in the documentation and/or other materials
*        provided with the distribution.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AWV
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "getopt.h"

#ifndef min
#  define min(x,y) (x)<(y) ? (x) : (y)
#endif // min

char* optarg = 0;
int   optind = 1;
int   opterr = 1;
int   optopt = '?';
static int nextchar = 0;

typedef struct _getopt_param {
	const char* optstring;	// effective option string, without prefixes
	bool posix;		// POSIX compatible mode (stop at first not detected parameter)
	bool process_all;	// process all elements
	bool no_report;		// don't report about errors...
	char missing_arg;	// character for return for missing option argument
				//   by default - '?', but may be ':'
} getopt_param;


// this function detects for parameters of processing
static getopt_param scan_param(const char* optstring)
{
	assert(optstring);
	getopt_param param = { optstring, false, false, false, '?' };
	size_t len = strlen(optstring);

	if (!opterr)
		param.no_report = true;

	for (size_t i = 0; i < len; i++) {
		// if option string begins with symbols '-+:' then this is special symbols...
		switch(optstring[i])
		{
		case '-':
			param.process_all = true;
			break;
		case '+':
			param.posix = true;
			break;
		case ':':
			param.missing_arg = ':';
			param.no_report = true; // i don't know why, but if optstrings starts with
			break;                  // ':' - original Linux function doesn't report about errors...
		default: // ok, no more special symbols... save efective format string & return
			param.optstring = optstring + i;
			return param;
		}
	}
	assert(0); // no more symbols? not good :(
	return param;
}

// this function detects different between 2 strings:
// return values:
//   -1 - strings differ
//   0  - strings same
//   >0 - str is substring of opt
static int string_diff(const char* opt, const char* str, int len)
{
	assert(opt);
	assert(str);
	assert(len);
	assert(len <= (int)strlen(str));

	int olen = strlen(opt);

	if (olen < len)
		return -1;

	for (int i = 0;; i++) {
		if(!opt[i] && i == len)
			return 0; // option == str
		else if(opt[i] && i >= len)
			return olen - i; // str is part of option
		else if(opt[i] != str[i])
			return -1; // option is shorter str... in str specified option name longer that option
	}
}

typedef enum _opt_type
{
	opt_not_found		= 0,
	opt_err_no_arg		= 1,
	opt_err_ambiguous	= 2,
	opt_single		= 3,
	opt_inplace_arg		= 4,
	opt_has_arg		= 5
} opt_type;


// check for short option... should be like "-opt_name"
static bool is_short_option(const char* str)
{
	return (str[0] == '-' && str[1] && str[1] != '-');
}

// check for long option... should be like "--opt_name"
static bool is_long_option(const char* str)
{
	return (str[0] == '-' && str[1] == '-'  && str[2]);
}

static bool is_option(const char* str)
{
	return (is_short_option(str) || is_long_option(str));
}

static bool is_finish(const char* str)
{ // if string == "--" - then this is terminator (no more process)
	return (str[0] == '-' && str[1] == '-' && !str[2]);
}

static int short_option_begin(const char* /*str*/)
{ // currently this is fake because supported only '-' as attribute of short option
	return 1;
}

static int long_option_begin(const char* /*str*/)
{ // currently this is fake because supported only '--' as attribute of long option
	return 2;
}

static int option_begin(const char* str)
{
	if (is_short_option(str))
		return short_option_begin(str);
	else if (is_long_option(str))
		return long_option_begin(str);
	assert(0);
	return 0;
}

// looking for nearest option or terminate sequence ('--') in argv from optind index
// if no option found - return -1, else - index of found option
static int look_for_option(int argc, char* const argv[], int optind)
{
	assert(argv);
	for (int i = optind; i < argc; i++)
		if (is_option(argv[i]) || is_finish(argv[i]))
			return i;
	return -1;
}

// this function tries to detect short option entry in string
static opt_type check_for_short_option(const char* optstring, const char* str)
{
	assert(optstring);
	assert(str);

	size_t olen = strlen(optstring);
	size_t slen = strlen(str);

	if (!olen || !slen)
		return opt_not_found;

	char sym = str[0];
	for (size_t i = 0; i < olen; i++) { // loop for all options in optstring
		if (optstring[i] != ':' && optstring[i] == sym) {
			// ok, we found option...
			// let's check is it valid?
			if (optstring[i + 1] != ':') {
				// single option... just return...
				return opt_single;
			}
			else if (optstring[i + 1] == ':' && optstring[i + 2] != ':') {
				// mandatory argument
				if (str[1]) // value of arg is in same argv
					return opt_inplace_arg;
				else // else next arg is value of option
					return opt_has_arg;
			}
			else {
				// if(optstring[i+1] == ':' && optstring[i+2] == ':')
				// optional argument
				if (str[1]) // value of arg is in same argv
					return opt_inplace_arg;
				else // else optional argument is absent
					return opt_single;
			}
		}
	}

	return opt_not_found;
}

// this function tries to detect long option entry in string
static opt_type check_for_long_option(const struct option *longopts, int *longindex,
	const char* str, const char*& arg)
{
	assert(longindex);
	assert(str);

	arg = 0;

	if (!longopts)
		return opt_not_found;

	// looking for '=' in string...
	int i;
	for (i = 0; str[i] && str[i] != '='; i++);
	assert(str[i] == '=' || !str[i]);

	int len = i; // len used for detecting length of option name in string

	if (str[i] == '=' && str[i + 1]) // ok, symbol '=' detected...
		arg = str + i + 1; // set potential option name substring length

	// detecting the nearest option name to string
	int index = 1;
	const char* opt = longopts->name;

	if (!opt)
		return opt_not_found;

	int diff = string_diff(opt, str, len);
	int selected = diff >= 0 ? 0 : -1;
	bool ambiguous = false;

	while (longopts[index].name) {
		opt = longopts[index].name;

		int _diff = string_diff(opt, str, len);
		if (!_diff) {
			diff = 0;
			selected = index;
			break;
		}

		if (_diff > 0 && diff > 0)
			ambiguous = true; // detected 2 or more options which are confirms string

		else if (_diff > 0 && (_diff < diff || diff < 0)) {
			diff = _diff;
			selected = index;
		}
		index++;
	}

	if (ambiguous && diff > 0) // if detected multiple confirms and no exact match - return error
		return opt_err_ambiguous;

	if (selected >= 0) {
		if (longopts[selected].has_arg == no_argument && arg) {
			return opt_err_no_arg; // argument is not required but specified
		}
		else if (longopts[selected].has_arg == no_argument /*&& !arg*/) {
			*longindex = index;
			return opt_single; // option without argument
		}
		else if (longopts[selected].has_arg == required_argument && arg) {
			*longindex = index;
			return opt_inplace_arg;
		}
		else if (longopts[selected].has_arg == required_argument) {
			*longindex = index;
			return opt_has_arg;
		}
		else if (longopts[selected].has_arg == optional_argument && arg) {
			*longindex = index;
			return opt_inplace_arg;
		}
		else if (longopts[selected].has_arg == optional_argument) {
			*longindex = index;
			return opt_single; // option without argument
		}
	}
	return opt_not_found;
}

// here stored all numbers of elements which should be placed at end of argv list
static int* swap_list = 0;
static int  swap_num = 0;

static void clear_swap()
{
	if (swap_list)
		free(swap_list);
	swap_list = 0;
	swap_num = 0;
}

// adding index to list
static void add_to_swap(int idx, int num = 1)
{
	assert(!swap_list && !swap_num || swap_list && swap_num);
	assert(num);

	if (!swap_num)
		swap_list = (int*)malloc(sizeof(*swap_list) * num);
	else
		swap_list = (int*)realloc(swap_list, sizeof(*swap_list) * (swap_num + num));
	assert(swap_list);

	for (int i = 0; i < num; i++)
		swap_list[swap_num + i] = idx + i;
	swap_num += num;
}

// swap values in array
// this function moves elements enumrated in swap_list to end of
// argv vector...
// arguments:
//   argc   - size of argv vector (used mostly for control of length,
//            to be sure that we are inside of vector)
//   argv   - vector to process
//   optind - bound of process... argv vector is processed in range 0..optind
static void swap_values(int argc, char** argv, int optind)
{
	assert(optind >= swap_num);
	assert(optind <= argc);

	int i; // index for original array
	int j; // index for temp array
	int k; // index for swap array

	int vals = min(argc, optind);

	char** tmp = (char**)malloc(vals * sizeof(*tmp));
	assert(tmp);

	for (i = j = k = 0; i < vals && j < vals; i++) {
		// copying all elements which NOT enumerated in swap_list array to temp array
		if(k >= swap_num || i != swap_list[k]) {
			tmp[j++] = argv[i];
		}
		else {// if index of element is in swap_list array - then just skip this element
		      // NOTE: swap_list array is SORTED, that is why we can to not run
		      //       throgh whole swap_list array in every iteration
			assert(k < swap_num);
			k++;
		}
	}

	// ok, now copy rest of array (skipped elements) to temp array
	for (k = 0; j < vals && k < swap_num; k++, j++) {
		assert(swap_list[k] < optind);
		assert(swap_list[k] < argc);
		tmp[j] = argv[swap_list[k]];
	}

	// and now save tmp array to argv
	for (i = 0; i < vals; i++)
		argv[i] = tmp[i];

	free(tmp);

	::optind -= swap_num;

	clear_swap();
}

// this function is called when we need to detect next option entry in argv
// return value:
//   true  - next option entry found, retval not used
//   false - no more options, retval - reason (-1 no more options, 1 - argument processed as default
//                                             option with code 1)
static bool detect_next_option(int argc, char* const argv[], const getopt_param& param, int& retval)
{
	if (!is_option(argv[optind])) {
		// if optind points to non-option - then check it...
		if (is_finish(argv[optind])) {
			// is it terminate sequence ('--')?
			optind++;
			if(!param.posix && !param.process_all)
				swap_values(argc, (char**)argv, optind);
			retval = -1;
			return false;
		}
		else if (param.process_all) {// processing all elements
			optarg = (char*)argv[optind];
			optind++;
			retval = 1;
			return false;
		}
		else if (param.posix) {
			retval = -1;
			return false;
		}
		else {
			int next_ind = look_for_option(argc, argv, optind);
			assert(next_ind != optind); // should not be same
			if (next_ind > 0) {
				add_to_swap(optind, next_ind - optind); // marking elements for swap
						    /* NOTE: in swap_list stored indexes of elements which are not
						    detected as options and not detected as arguments
						    (if argv[x] is detected as option - stsrts with '-',
						    it doesn't added to this list). later, when no more
						    element may be detected, these stored elements are moved
						    to end of argv vector */
				optind = next_ind;
			}
			else {// no more options found
				swap_values(argc, (char**)argv, optind);
				retval = -1;
				return false;
			}
		}

		if (is_finish(argv[optind])) {
			optind++;
			if (!param.posix && !param.process_all)
				swap_values(argc, (char**)argv, optind);
			retval = -1;
			return false;
		}
	}
	return true;
}

// this function is called when we are sure that option is short...
static int get_short_option(int argc, char* const argv[], const getopt_param& param)
{
	assert(argc);
	assert(argv);
	assert(nextchar);
	assert(optind < argc);

	const char* str = argv[optind] + nextchar;

	switch(check_for_short_option(param.optstring, str))
	{
	case opt_not_found:
		optopt = str[0];
		nextchar++;
		if (!argv[optind][nextchar]) {
			nextchar = 0;
			optind++;
		}
		if(!param.no_report)
			printf("%s: invalid option -- %c\n", argv[0], optopt);
		return '?';
	case opt_err_no_arg:
		// somethibg wrong... check_for_short_option can't
		// detect absent external parameter...
		assert(0);
		break;
	case opt_single:
		nextchar++;
		if (!argv[optind][nextchar]) {
			nextchar = 0;
			optind++;
		}
		return str[0];
	case opt_inplace_arg:
		optarg = (char*)str + 1;
		optind++;
		nextchar = 0;
		return str[0];
	case opt_has_arg:
		if(optind + 1 >= argc) {
			// error: option hasn't argument
			optopt = str[0];
			optind++;
			nextchar = 0;
			if(!param.no_report)
				printf("%s: option requires an argument -- %c\n", argv[0], optopt);
			return param.missing_arg;
		}
		else {
			optarg = (char*)argv[optind + 1];
			optind += 2;
			nextchar = 0;
			return str[0];
		}
	}

	// should not go here...
	assert(0);
	return -1;
}

static int get_long_option(int argc, char* const argv[], const getopt_param& param,
	const option* longopts, int* longindex)
{
	assert(argc);
	assert(argv);
	assert(!nextchar);
	assert(optind < argc);

	optopt = 0;
	*longindex = 0;

	const char* element = argv[optind]; // saving current argument for error message

	switch(check_for_long_option(longopts, longindex,
		argv[optind] + option_begin(argv[optind]),
		(const char*&)optarg))
	{
	case opt_not_found:
		if (!param.no_report)
			printf("%s: unrecognized option `%s'\n", argv[0], element);
		optind++;
		return '?';
	case opt_err_no_arg:
		if (!param.no_report)
			printf("%s: option `%s' doesn't allow an argument\n", argv[0], element);
		optind++;
		return '?';
	case opt_has_arg:
		if (optind + 1 >= argc) {
			optind++;
			if (!param.no_report)
				printf("%s: option `%s' requires an argument\n", argv[0], element);
			*longindex = 0;
			return param.missing_arg;
		}
		optarg = (char*)(argv[optind + 1]);
		optind++;
		// here should NOT be break because here is same return method
	case opt_single:
	case opt_inplace_arg:
		optind++;
		if (longopts[*longindex].flag) {
			*longopts[*longindex].flag = longopts[*longindex].val;
			return 0;
		}
		else {
			return longopts[*longindex].val;
		}
	case opt_err_ambiguous:
		optind++;
		if (!param.no_report)
			printf("%s: option `%s' is ambiguous\n", argv[0], element);
		return '?';
	}

	assert(0); // should not be here...
	return -1;
}

static int get_long_short_option(int argc, char* const argv[], const getopt_param& param,
	const option* longopts, int* longindex)
{
	assert(argc);
	assert(argv);
	assert(!nextchar);
	assert(optind < argc);

	optopt = 0;
	*longindex = 0;

	const char* element = argv[optind]; // saving current argument for error message

	opt_type ltype = check_for_long_option(longopts, longindex,
		argv[optind] + option_begin(argv[optind]),
		(const char*&)optarg);
	opt_type stype = check_for_short_option(param.optstring,
		argv[optind] + option_begin(argv[optind]));

	if (ltype == opt_not_found && stype == opt_not_found) {
		if(!param.no_report)
			printf("%s: unrecognized option `%s'\n", argv[0], element);
		optind++;
		return '?';
	}
	else if (ltype != opt_not_found) {
		return get_long_option(argc, argv, param, longopts, longindex);
	}
	else {//if(stype != opt_not_found)
		nextchar = short_option_begin(argv[optind]);
		return get_short_option(argc, argv, param);
	}
}

int getopt(int argc, char* const argv[], const char *optstring)
{
	if (!argc || !argv || !optstring)
		return -1;

	getopt_param param = scan_param(optstring ? optstring : "");

	if (optind >= argc) {
		if (!param.posix && !param.process_all)
			swap_values(argc, (char**)argv, optind);
		return -1;
	}

	optarg = 0;

	if (!nextchar) {// we are in begin of argument... no previous iterations on this arg
		int retval;
		if (!detect_next_option(argc, argv, param, retval))
			return retval;
		nextchar = short_option_begin(argv[optind]);
	}

	return get_short_option(argc, argv, param);
}

int getopt_long(int argc, char * const argv[],
	const char *optstring,
	const struct option *longopts, int *longindex)
{
	if (!argc || !argv)
		return -1;

	getopt_param param = scan_param(optstring ? optstring : "");

	if (optind >= argc) {
		if (!param.posix && !param.process_all)
			swap_values(argc, (char**)argv, optind);
		return -1;
	}

	optarg = 0;

	if (!nextchar) {// we are in begin of argument... no previous iterations on this arg
		int retval;
		if (!detect_next_option(argc, argv, param, retval))
			return retval;

		const char* str = argv[optind];
		if (is_long_option(str)) {
			int _longindex;
			if(!longindex) // if no longindex specified - create own temp...
				longindex = &_longindex;
			return get_long_option(argc, argv, param, longopts, longindex);
		}
		else if(is_short_option(str)) {
			nextchar = short_option_begin(str);
		}
	}

	return get_short_option(argc, argv, param);
}

int getopt_long_only(int argc, char * const argv[],
	const char *optstring,
	const struct option *longopts, int *longindex)
{
	if (!argc || !argv)
		return -1;

	getopt_param param = scan_param(optstring ? optstring : "");

	if (optind >= argc) {
		if (!param.posix && !param.process_all)
			swap_values(argc, (char**)argv, optind);
		return -1;
	}

	optarg = 0;

	if (!nextchar) {// we are in begin of argument... no previous iterations on this arg
		int retval;
		if (!detect_next_option(argc, argv, param, retval))
			return retval;

		const char* str = argv[optind];
		if (is_long_option(str)) {
			int _longindex;
			if (!longindex) // if no longindex specified - create own temp...
				longindex = &_longindex;
			return get_long_option(argc, argv, param, longopts, longindex);
		}
		else if (is_short_option(str)) {
			return get_long_short_option(argc, argv, param, longopts, longindex);
		}
	}

	return get_short_option(argc, argv, param);
}
