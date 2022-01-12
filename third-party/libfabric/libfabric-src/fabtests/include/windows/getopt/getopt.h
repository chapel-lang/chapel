/*
 * Copyright (c) 2017 Intel Corporation.  All rights reserved.
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
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _GETOPT_H_
#define _GETOPT_H_

#ifdef __cplusplus
extern "C" {
#endif __cplusplus

#if defined WIN32 && !defined GETOPT_STATIC
#ifdef GETOPT_DLL_EXPORTS
#define GET_OPT_INTERFACE __declspec(dllexport)
#else // GETOPT_DLL_EXPORTS
#define GET_OPT_INTERFACE __declspec(dllimport)
#endif // GETOPT_DLL_EXPORTS
#else // WIN32 && not defined GETOPT_STATIC
#define GET_OPT_INTERFACE
#endif

GET_OPT_INTERFACE extern char* optarg;
extern int   optind, opterr, optopt;

#ifndef no_argument
# define no_argument		0
#endif //no_argument

#ifndef required_argument
# define required_argument	1
#endif //required_argument

#ifndef optional_argument
# define optional_argument	2
#endif //optional_argument

int getopt(int argc, char *const argv[], const char *optstring);

struct option
{
	const char *name;
	int has_arg;
	int *flag;
	int val;
};

int getopt_long(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex);

int getopt_long_only(int argc, char *const argv[],
		     const char *optstring,
		     const struct option *longopts, int *longindex);

#ifdef __cplusplus
}
#endif __cplusplus

#endif //_GETOPT_H_
