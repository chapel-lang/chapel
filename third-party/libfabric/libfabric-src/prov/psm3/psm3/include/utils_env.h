/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2022 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2022 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef UTILS_ENV_H
#define UTILS_ENV_H

#include "psm2_mock_testing.h"
#include "fnmatch.h"

/* we can only include low level headers here because this is
 * #included by utils_sysfs.c.  Can't pull in HAL headers or heap debug macros
 */

// we front end getenv with a check in the psm3.conf file
#define PSM3_ENV_FILENAME "/etc/psm3.conf"

int psm3_env_initialize(void);
void psm3_env_finalize(void);
char* psm3_env_get(const char *name);

/*
 * Parsing environment variables
 */

union psmi_envvar_val {
	void *e_void;
	char *e_str;
	int e_int;
	unsigned int e_uint;
	long e_long;
	unsigned long e_ulong;
	unsigned long long e_ulonglong;
};

// psm3_getenv only expects LEVEL
// psm3_getenv_range accepts LEVEL and FLAGs
// MIN/MAX N/A to TYPEs: YESNO, STR, STR_VAL_PAT_*, STR_TUPLES
// 'min' and 'max' only allowed as input when corresponding
// range check enabled
// FLAG_FATAL will cause a fatal error on invalid input
// (syntax, range or check function detected).  When FLAG_FATAL is not
// set an invalid input will fallback to the default with a message.
#define PSMI_ENVVAR_LEVEL_USER	         1	// show in user help
#define PSMI_ENVVAR_LEVEL_HIDDEN         2	// hidden from user help
#define PSMI_ENVVAR_LEVEL_NEVER_PRINT    4	// a bit flag, never show in help
#define PSMI_ENVVAR_LEVEL_MASK           0x07	// mask for getting level
#define PSMI_ENVVAR_FLAG_NOMIN           0x10	// no min check
#define PSMI_ENVVAR_FLAG_NOMAX           0x20	// no max check
#define PSMI_ENVVAR_FLAG_NOABBREV        0x40	// no 'min' or 'max' as input
#define PSMI_ENVVAR_FLAG_NOMIN_NOMAX     0x70	// no min, no max, no abbrev
#define PSMI_ENVVAR_FLAG_FATAL           0x80	// invalid input is fatal

#define PSMI_ENVVAR_TYPE_YESNO					0
#define PSMI_ENVVAR_TYPE_STR					1
#define PSMI_ENVVAR_TYPE_INT					2
#define PSMI_ENVVAR_TYPE_UINT					3
#define PSMI_ENVVAR_TYPE_UINT_FLAGS				4
#define PSMI_ENVVAR_TYPE_LONG					5
#define PSMI_ENVVAR_TYPE_ULONG					6
#define PSMI_ENVVAR_TYPE_ULONG_FLAGS			7
#define PSMI_ENVVAR_TYPE_ULONG_ULONG			8
#define PSMI_ENVVAR_TYPE_STR_VAL_PAT_INT		9
#define PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT		10
#define PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT_FLAGS	11
#define PSMI_ENVVAR_TYPE_STR_TUPLES				12

#define PSMI_ENVVAR_VAL_YES ((union psmi_envvar_val) 1)
#define PSMI_ENVVAR_VAL_NO  ((union psmi_envvar_val) 0)

void psm3_env_print_val(FILE *f, const char *name, int type,
						union psmi_envvar_val val);
int psm3_env_snprint_val(char *buf, size_t size, const char *name, int type,
						union psmi_envvar_val val);

// psm3_getenv_check_t is optional in psm3_getenv_range
// to confirm the resulting value is valid (return of 0).
// On error (return != 0), errstr[errstr_size] is filled in with
// '\0' terminated string with more information about the error.
//
// This may be used for any envvar type to do further checks of the value
// such as integers which may need to be power of 2 or parse checking
// of strings.
// For strings parsed value(s) is not returned, so caller will need to parse
// again, but this allows better error reporting during env variable get.
//
// ptr is caller specific and can pass additional input information which may
// assist in verification of values. ptr should be used as input only
// because the check function is only called by psm3_getenv_range when
// otherwise valid input is supplied.
typedef int (*psm3_getenv_check_t)(int type, const union psmi_envvar_val val,
							 void *ptr, size_t errstr_size, char errstr[]);

int
MOCKABLE(psm3_getenv)(const char *name, const char *descr, int level,
		int type, union psmi_envvar_val defval,
		union psmi_envvar_val *newval);
MOCK_DCL_EPILOGUE(psm3_getenv);

// NOTE: This function writes the entire output union pointed to by newval. as a
// result, the backing storage for the pointer must be at least the size of the
// full union type, not simply the size of the type indicated by the type
// parameter.
//
int MOCKABLE(psm3_getenv_range)(const char *name, const char *descr,
		const char *help, unsigned level_flags,
		int type, union psmi_envvar_val defval, union psmi_envvar_val min,
		union psmi_envvar_val max, psm3_getenv_check_t check, void *ptr,
		union psmi_envvar_val *newval);
MOCK_DCL_EPILOGUE(psm3_getenv_range);

int psm3_count_tuples(const char *str);

/*
 * Parsing int, unsigned int and long parameters
 * 0 -> ok, *val updated
 * -1 -> empty string, *val not updated
 * -2 -> parse error, *val not updated
 */
int psm3_parse_str_int(const char *string, int *val, int min, int max);
int psm3_parse_str_uint(const char *string, unsigned int *val,
						unsigned int min, unsigned int max);
int psm3_parse_str_long(const char *str, long *val, long min, long max);

/*
 * Parsing yesno parameters
 * allows: yes/no, true/false, on/off, 1/0
 * 0 -> ok, *val updated
 * -1 -> empty string, *val not updated
 * -2 -> parse error, *val not updated
 */
int psm3_parse_str_yesno(const char *str, int *val);

/*
 * Parsing int parameters set in string tuples.
 * Returns:
 * 0 - parsed with no errors, vals[] updated
 * -1 - empty or NULL string, vals[] unchanged
 * -2 -  syntax error in one of more of the parameters
 *        parameters with syntax errors are unchanged, others without
 *        syntax errors are updated in vals[]
 */
int psm3_parse_str_tuples(const char *str, int ntup, int *vals);

/* parse env of the form 'val' or 'val:' or 'val:pattern'
 * Returns:
 * 0 - parsed and matches current process, *val set to parsed val
 * 0 - parsed and doesn't match current process, *val set to def
 * -1 - nothing provided, *val set to def
 * -2 - syntax error, *val set to def
 * flags PSMI_ENVVAR_FLAG_NOMIN, PSMI_ENVVAR_FLAG_NOMAX and
 * PSMI_ENVVAR_FLAG_NOABBREV control if 'min', 'minimum', 'max' or 'maximum'
 * allowed as input and indicate if min and/or max supplied.
 */
int psm3_parse_val_pattern_int(const char *env, int def, int *val,
					unsigned flags, int min, int max);
int psm3_parse_val_pattern_uint(const char *env, unsigned def, unsigned *val,
					unsigned flags, unsigned min, unsigned max);

#if defined(PSM_VERBS) || defined(PSM_SOCKETS)
// return forced speed in mbps or 0 if not forced
unsigned long psm3_parse_force_speed();
// this can be overridden in compiler args, default is 32,000 mbps (32 gbps)
// if set to 0, no default (lack of speed is then fatal)
#ifndef PSM_DEFAULT_SPEED
#define PSM_DEFAULT_SPEED 32000
#endif
#endif

// should PSM3 set the cpu affinity itself
int psm3_env_psm_sets_cpuaffinity(int skip_affinity);

#endif /* UTILS_ENV_H */
