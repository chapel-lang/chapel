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

#include <malloc.h>             /* malloc_usable_size */
#include <fnmatch.h>
#include <ctype.h>
#include "psm_user.h"

static struct psm3_env_var {
	char *name;
	char *value;
} *psm3_env;

static unsigned psm3_env_count;
static unsigned psm3_env_alloc;
static unsigned psm3_env_initialized;

#define PSM3_ENV_INC 20	// number of additional entries per env realloc

// Absolute max non-comment lines in psm3.conf
// There are less then 200 unique PSM3 env variables so 1024 is more than enough
#define PSM3_ENV_LIMIT 1024

// parse the /etc/psm3.conf file
// info prints are used to warn about malformed lines ignored
// this is called early in psm3_init() so all info prints will still occur
// but PSM3_VERBOSE_ENV can be exported or placed early in the file to
// enable prints of values as they are read
// returns 0 on success, -1 on error
int psm3_env_initialize(void)
{
	FILE *f;
	char buf[1024];
	int env_log_level = 1;	// log controlled by TRACEMASK
	unsigned verb_env_val = 0;

	if (psm3_env_initialized)
		return 0;	// already initialized

	// get verbosity level setting for env logging
	// if invalid syntax, will output warning when parse during psm3_getenv
	const char *verb_env = getenv("PSM3_VERBOSE_ENV");
	(void)psm3_parse_val_pattern_uint(verb_env, 0, &verb_env_val,
						PSMI_ENVVAR_FLAG_NOABBREV, 0, 3);
	if (verb_env_val)
		env_log_level = 0;	// log at INFO level

	f = fopen(PSM3_ENV_FILENAME, "r");
	if (f == NULL) {
		if (errno == ENOENT) {
			// file is optional
			_HFI_ENVDBG(env_log_level, "%s: not found\n", PSM3_ENV_FILENAME);
			return 0;
		} else {
			_HFI_ERROR("Unable to open %s: %s\n",
					PSM3_ENV_FILENAME, strerror(errno));
			return -1;
		}
	}
	while (fgets(buf, sizeof(buf), f) != NULL) {
		int l = strlen(buf);
		int i, j;
		struct psm3_env_var var;
		char *p;

		if (! l) continue;	// should not happen
		if (buf[l-1] != '\n') {
			// can also occur for last line if no newline at end
			int c;
			c = fgetc(f);
			if (c != EOF) {
				// line too long, fgetc until read newline
				_HFI_ENV_ERROR("%s: Ignoring line too long: '%s' ...\n",
						PSM3_ENV_FILENAME, buf);
				while (c != (int)(unsigned char)'\n' && (c = fgetc(f)) != EOF)
					;
				continue;
			}
		} else {
			buf[l-1] = '\0';	// drop newline
		}

		// drop any comment at end of line
		p = strchr(buf, '#');
		if (p)
			*p = '\0';

		// skip leading white space
		i = strspn(buf, " \t");

		if (buf[i] == '\0') continue;	// skip blank lines

		// drop any trailing whitespace
		l = strlen(&buf[i]);
		while (l>0 && isspace(buf[i+l-1])) {
			buf[i+l-1] = '\0';
			l--;
		}

		// length of variable name
		j = strspn(&buf[i], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
		if (buf[i+j] != '=') {
			// malformed assignment,skip
			_HFI_ENV_ERROR("%s: Ignoring malformed assignment: '%s'\n",
					PSM3_ENV_FILENAME, buf);
			continue;
		}
		buf[i+j] = '\0';

		if (psm3_env_count >= PSM3_ENV_LIMIT) {
			_HFI_ERROR("%s: Limit of %u entries\n",
						PSM3_ENV_FILENAME, PSM3_ENV_LIMIT);
			goto fail;
		}

		var.name = psmi_strdup(PSMI_EP_NONE, &buf[i]);
		if (! var.name) {
			_HFI_ERROR("%s: Unable to allocate memory for entry %s\n",
						PSM3_ENV_FILENAME, &buf[i]);
			goto fail;
		}
		// skip name and '=', rest is value
		var.value = psmi_strdup(PSMI_EP_NONE, &buf[i+j+1]);
		if (! var.value) {
			_HFI_ERROR("%s: Unable to allocate memory for entry %s\n",
						PSM3_ENV_FILENAME, &buf[i]);
			psmi_free(var.name);
			goto fail;
		}

		// allow /etc/psm3.conf to set PSM3_VERBOSE_ENV when defaulted
		// if invalid syntax, will output warning when parse during psm3_getenv
		if (! verb_env && 0 == strcmp("PSM3_VERBOSE_ENV", var.name)) {
			(void)psm3_parse_val_pattern_uint(var.value, 0, &verb_env_val,
						PSMI_ENVVAR_FLAG_NOABBREV, 0, 3);
			if (verb_env_val)
				env_log_level = 0;	// log at INFO level
		}
		// Note: we don't let PSM3_TRACEMASK affect this

		// this must be parsed in a constructor prior to this function,
		// so we ignore it here
		if (0 == strcmp(var.name, "PSM3_DISABLE_MMAP_MALLOC")) {
			_HFI_ENV_ERROR("WARNING: %s Ignoring %s\n", PSM3_ENV_FILENAME,var.name);
			psmi_free(var.name);
			psmi_free(var.value);
			continue;
		}

		_HFI_ENVDBG(env_log_level, "%s: parsed %s='%s'\n",
				PSM3_ENV_FILENAME, var.name, var.value);

		if (psm3_env_count >= psm3_env_alloc) {
			unsigned n = psm3_env_alloc + PSM3_ENV_INC;
			void *p = psmi_realloc(PSMI_EP_NONE, UNDEFINED, psm3_env,
									sizeof(psm3_env[0])*n);
			if (! p) {
				_HFI_ERROR("%s: Unable to allocate memory for %u entries\n",
							PSM3_ENV_FILENAME, n);
				psmi_free(var.name);
				psmi_free(var.value);
				goto fail;
			}
			psm3_env_alloc = n;
			psm3_env = (struct psm3_env_var*)p;
		}
		psm3_env[psm3_env_count++] = var;
	}
	if (! feof(f) || ferror(f)) {
		_HFI_ERROR("Error reading %s\n", PSM3_ENV_FILENAME);
		goto fail;
	}
	fclose(f);
	psm3_env_initialized = 1;
	return 0;

fail:
	psm3_env_finalize();
	fclose(f);
	return -1;
}

void psm3_env_finalize(void)
{
	int i;

	if (! psm3_env_initialized)
		return;

	for (i=0; i < psm3_env_count; i++) {
		psmi_free(psm3_env[i].name);
		psmi_free(psm3_env[i].value);
	}
	psmi_free(psm3_env);
	psm3_env = NULL;
	psm3_env_count = 0;
	psm3_env_alloc = 0;
	psm3_env_initialized = 0;
}

void psm3_env_print_val(FILE *f, const char *name, int type,
						union psmi_envvar_val val)
{
	switch (type) {
	case PSMI_ENVVAR_TYPE_STR:
	case PSMI_ENVVAR_TYPE_STR_TUPLES:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_INT:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT_FLAGS:
		fprintf(f, "%s=%s\n", name, val.e_str);
		break;
	case PSMI_ENVVAR_TYPE_INT:
	case PSMI_ENVVAR_TYPE_YESNO:
		fprintf(f, "%s=%d\n", name, val.e_int);
		break;
	case PSMI_ENVVAR_TYPE_UINT:
		fprintf(f, "%s=%u\n", name, val.e_uint);
		break;
	case PSMI_ENVVAR_TYPE_UINT_FLAGS:
		fprintf(f, "%s=0x%x\n", name, val.e_uint);
		break;
	case PSMI_ENVVAR_TYPE_LONG:
		fprintf(f, "%s=%ld\n", name, val.e_long);
		break;
	case PSMI_ENVVAR_TYPE_ULONG:
		fprintf(f, "%s=%lu\n", name, val.e_ulong);
		break;
	case PSMI_ENVVAR_TYPE_ULONG_FLAGS:
		fprintf(f, "%s=0x%lx\n", name, val.e_ulong);
		break;
	case PSMI_ENVVAR_TYPE_ULONG_ULONG:
		fprintf(f, "%s=%llu\n", name, val.e_ulonglong);
		break;
	}
}

int psm3_env_snprint_val(char *buf, size_t size, const char *name, int type,
						union psmi_envvar_val val)
{
	switch (type) {
	case PSMI_ENVVAR_TYPE_STR:
	case PSMI_ENVVAR_TYPE_STR_TUPLES:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_INT:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT_FLAGS:
		return snprintf(buf, size, "%s=%s\n", name, val.e_str);
		break;
	case PSMI_ENVVAR_TYPE_INT:
	case PSMI_ENVVAR_TYPE_YESNO:
		return snprintf(buf, size, "%s=%d\n", name, val.e_int);
		break;
	case PSMI_ENVVAR_TYPE_UINT:
		return snprintf(buf, size, "%s=%u\n", name, val.e_uint);
		break;
	case PSMI_ENVVAR_TYPE_UINT_FLAGS:
		return snprintf(buf, size, "%s=0x%x\n", name, val.e_uint);
		break;
	case PSMI_ENVVAR_TYPE_LONG:
		return snprintf(buf, size, "%s=%ld\n", name, val.e_long);
		break;
	case PSMI_ENVVAR_TYPE_ULONG:
		return snprintf(buf, size, "%s=%lu\n", name, val.e_ulong);
		break;
	case PSMI_ENVVAR_TYPE_ULONG_FLAGS:
		return snprintf(buf, size, "%s=0x%lx\n", name, val.e_ulong);
		break;
	case PSMI_ENVVAR_TYPE_ULONG_ULONG:
		return snprintf(buf, size, "%s=%llu\n", name, val.e_ulonglong);
		break;
	}
	psmi_assert_always(0);
	return 0;
}

// check getenv first and then psm3_env (/etc/psm3.conf)
char *psm3_env_get(const char *name)
{
	char *ret = getenv(name);
	int i;

	if (ret || ! psm3_env_initialized || ! psm3_env_count)
		return ret;

	for (i=psm3_env_count-1; i >= 0;  i--) {
		if (0 == strcmp(name, psm3_env[i].name))
			return psm3_env[i].value;
	}
	return NULL;
}

// don't document that 3 and 3: and 3:pattern can output hidden params
const char *PSM3_VERBOSE_ENV_HELP =
	"Enable verbose output of environment variables.\n"
	"  0 - none\n"
	"  1 - only output changed w/o help\n"
	"  2 - output all with help,\n"
	"  #: - limit output to rank 0\n"
	"  #:pattern - limit output to processes whose label matches\n    "
#ifdef FNM_EXTMATCH
		"extended "
#endif
		"glob pattern";

/* If PSM3_VERBOSE_ENV is set in the environment, we determine
 * what its verbose level is and print the environment at "INFO"
 * level if the environment's level matches the desired printlevel.
 */
static int psmi_getenv_verblevel = -1;
static int psm3_getenv_is_verblevel(int printlevel)
{
	if (psmi_getenv_verblevel == -1) {
		// first call, parse PSM3_VERBOSE_ENV and set verbosity level
		// for other env vars (and itself)
		char *env = psm3_env_get("PSM3_VERBOSE_ENV");
		int nlevel = PSMI_ENVVAR_LEVEL_USER;
		unsigned verb_env_val;
		if (env)
			psm3_stats_print_env("PSM3_VERBOSE_ENV", env);
		int ret = psm3_parse_val_pattern_uint(env, 0, &verb_env_val,
						PSMI_ENVVAR_FLAG_NOABBREV, 0, 3);
		psmi_getenv_verblevel = verb_env_val;
		if (psmi_getenv_verblevel > 0)
			nlevel = 0; /* output at INFO level */
		if (ret == -2)
			_HFI_ENVDBG(0, "Invalid value for %s ('%s') Using: %u\nHelp: %s\n",
				"PSM3_VERBOSE_ENV", env, verb_env_val, PSM3_VERBOSE_ENV_HELP);
		else if (psmi_getenv_verblevel == 1)
			_HFI_ENVDBG(0, " %-25s => '%s' (default was '%s')\n",
				"PSM3_VERBOSE_ENV", env?env:"", "0");
		else if (env && *env)
			_HFI_ENVDBG(nlevel, " %-25s => '%s' (default was '%s')\nHelp: %s\n",
				"PSM3_VERBOSE_ENV", env, "0", PSM3_VERBOSE_ENV_HELP);
		else	/* defaulted */
			_HFI_ENVDBG(nlevel,
				" %-25s => '%s'\nHelp: %s\n",
				"PSM3_VERBOSE_ENV", "0", PSM3_VERBOSE_ENV_HELP);
	}
	// printlevel is visibility of env (USER=1 or HIDDEN=2)
	// so at verbosity 1 and 2 output USER
	// at verbosity 3 output USER and HIDDEN
	return ((printlevel <= psmi_getenv_verblevel
			&& psmi_getenv_verblevel == 1)
		|| printlevel <= psmi_getenv_verblevel-1);
}

#define GETENV_PRINTF(_level, _fmt, ...)				\
	do {								\
		/* NEVER_PRINT disables output for deprecated variables */ \
		if ((_level & PSMI_ENVVAR_LEVEL_NEVER_PRINT) == 0)	\
		{							\
			int nlevel = _level;				\
			/* when enabled by VERBOSE_ENV, output at info (0), */ \
			/* otherwise output at nlevel (USER=1, HIDDEN=2 */ \
			if (psm3_getenv_is_verblevel(nlevel))		\
				nlevel = 0; /* output at INFO level */	\
			_HFI_ENVDBG(nlevel, _fmt, ##__VA_ARGS__);	\
		}							\
	} while (0)

// count number of fields in a str_tuple (field:field:....)
// The number is number of colons + 1
int psm3_count_tuples(const char *str)
{
	int ret = 1;
	if (! str)
		return 0;
	while (*str) {
		if (*str == ':')
			ret++;
		str++;
	}
	return ret;
}

/* _CONSUMED_ALL indicates if strtol() (and friends) consumed all of the input
 * passed to it. CHAR_PTR is the output char pointer from strtol
 */
#define _CONSUMED_ALL(CHAR_PTR) (((CHAR_PTR) != NULL) && (*(CHAR_PTR) == 0))

/* convert a string to a signed number with basic bounds checking
 * returns 0 - valid value and *val updated
 * -1 -> empty string, *val unchanged
 * -2 -> parse or range error, *val unchanged
 */
static int convert_str_signed(const char *str, long long *val,
					long long min, long long max)
{
	char *ep;
	long long temp;

	psmi_assert(val != NULL);
	if (! str || ! *str)
		return -1;
	/* Avoid base 8 (octal) on purpose, so don't pass in 0 for radix */
	errno = 0;
	temp = strtoll(str, &ep, 10);
	if (! _CONSUMED_ALL(ep)) {
		errno = 0;
		temp = strtoll(str, &ep, 16);
		if (! _CONSUMED_ALL(ep))
			return -2;
	}
	if (errno || temp < min || temp > max)
		return -2;

	*val = temp;
	return 0;
}

/* convert a string to an unsigned number with basic bounds checking
 * returns 0 - valid value and *val updated
 * -1 -> empty string, *val unchanged
 * -2 -> parse or range error, *val unchanged
 */
static int convert_str_unsigned(const char *str, unsigned long long *val,
					unsigned long long min, unsigned long long max)
{
	char *ep;
	unsigned long long temp;

	psmi_assert(val != NULL);
	if (! str || ! *str)
		return -1;
	/* Avoid base 8 (octal) on purpose, so don't pass in 0 for radix */
	errno = 0;
	temp = strtoull(str, &ep, 10);
	if (! _CONSUMED_ALL(ep)) {
		errno = 0;
		temp = strtoull(str, &ep, 16);
		if (! _CONSUMED_ALL(ep))
			return -2;
	}
	if (errno || temp < min || temp > max)
		return -2;

	*val = temp;
	return 0;
}
#undef _CONSUMED_ALL

// returns:
// 0 - valid value input
// 1 - variable not set, used default
// -1 - invalid value for variable or invalid syntax, used default
int MOCKABLE(psm3_getenv_range)(const char *name, const char *descr,
	    const char *help, unsigned level_flags,
	    int type, union psmi_envvar_val defval, union psmi_envvar_val min,
	    union psmi_envvar_val max, psm3_getenv_check_t check, void *ptr,
	    union psmi_envvar_val *newval)
{
	int ret = 0;
	union psmi_envvar_val tval;
	char *env = psm3_env_get(name);
	unsigned level = level_flags & PSMI_ENVVAR_LEVEL_MASK;
	char rangestr[80] = "";	// for help
	char errstr[512] = "";	// additional info for invalid values
	char statserrstr[700] = "";	// add'l info for stats file when invalid input

#define FORMAT_RANGESTR(FIELD, fmt) \
	do {																	\
		if ((level_flags & PSMI_ENVVAR_FLAG_NOMIN)) {						\
			if ((level_flags & PSMI_ENVVAR_FLAG_NOMAX))						\
				rangestr[0] = '\0';											\
			else															\
				snprintf(rangestr, sizeof(rangestr)," Max allowed " fmt "%s",\
					max.FIELD,												\
					(level_flags & PSMI_ENVVAR_FLAG_NOABBREV)?"":" (or 'max')");\
		} else if ((level_flags & PSMI_ENVVAR_FLAG_NOMAX)) {				\
			snprintf(rangestr, sizeof(rangestr)," Min allowed " fmt "%s",	\
					min.FIELD,												\
					(level_flags & PSMI_ENVVAR_FLAG_NOABBREV)?"":" (or 'min')");\
		} else {															\
			snprintf(rangestr, sizeof(rangestr)," Valid range " fmt "%s"	\
 					" to " fmt "%s",										\
					min.FIELD,												\
					(level_flags & PSMI_ENVVAR_FLAG_NOABBREV)?"":" (or 'min')",\
					max.FIELD,												\
					(level_flags & PSMI_ENVVAR_FLAG_NOABBREV)?"":" (or 'max')");\
		}																	\
	} while (0)

#define _GETENV_CHECK(tval)													\
	do {																	\
		if (check) {														\
			if ((*check)(type, tval, ptr, sizeof(errstr), errstr)) {				\
				tval = defval;												\
				ret = -1;													\
				/* errstr now has additional error information */			\
			}																\
		}																	\
	} while (0);

	/* for verblevel 1 we only output non-default values with no help
	 * for verblevel>1 we promote to info (verblevel=2 promotes USER,
	 *		verblevel=3 promotes HIDDEN) and show help.
	 * for verblevel< 1 we don't promote anything and show help
	 */
#define _GETENV_PRINT(env, ret, fmt, val, defval) \
	do {	\
		(void)psm3_getenv_is_verblevel(level);			\
		if (ret < 0 && (level_flags & PSMI_ENVVAR_FLAG_FATAL)) { \
			_HFI_ENV_ERROR("Invalid value for %s ('%s')%s\nHelp: %s%s\n%s%s", \
				name, env, errstr, descr, rangestr,\
				help?help:"", help?"\n":"");		\
			snprintf(statserrstr, sizeof(statserrstr), 	\
				"Invalid value ('%s')%s", env, errstr);			\
		} else if (ret < 0) {				\
			_HFI_ENV_ERROR("Invalid value for %s ('%s')%s Using: " fmt "\nHelp: %s%s\n%s%s", \
				name, env, errstr, val, descr, rangestr,\
				help?help:"", help?"\n":"");		\
			snprintf(statserrstr, sizeof(statserrstr), 	\
				"Invalid value ('%s')%s Using: " fmt, \
				env, errstr, val);			\
		} else if (ret > 0 && psmi_getenv_verblevel != 1)		\
			GETENV_PRINTF(level, "%s%-25s => " fmt	\
				"\nHelp: %s%s\n%s%s", level > 1 ? "*" : " ", name,	\
				val, descr, rangestr,			\
				help?help:"", help?"\n":"");\
		else if (ret == 0 && psmi_getenv_verblevel == 1)	\
			GETENV_PRINTF(1, "%s%-25s => "			\
				fmt " (default was " fmt ")\n",	\
				level > 1 ? "*" : " ", name,		\
				val, defval);		\
		else if (ret == 0 && psmi_getenv_verblevel != 1)	\
			GETENV_PRINTF(1, "%s%-25s => "		\
				fmt " (default was " fmt ")\nHelp: %s%s\n%s%s",	\
				level > 1 ? "*" : " ", name,		\
				val, defval, descr, rangestr,		\
				help?help:"", help?"\n":"");		\
	} while (0)

#define _CONVERT_TO_NUM(FIELD,TYPE,SIGNED,MIN,MAX)					\
	do {																	\
		if (!(level_flags & (PSMI_ENVVAR_FLAG_NOMIN|PSMI_ENVVAR_FLAG_NOABBREV))\
				&& (!strcasecmp(env, "min") || !strcasecmp(env, "minimum")))\
			tval.FIELD = min.FIELD;											\
		else if (!(level_flags & (PSMI_ENVVAR_FLAG_NOMAX|PSMI_ENVVAR_FLAG_NOABBREV))\
				&& (!strcasecmp(env, "max") || !strcasecmp(env, "maximum")))\
			tval.FIELD = max.FIELD;											\
		else {																\
			SIGNED long long temp;											\
			if (convert_str_##SIGNED(env, &temp, MIN, MAX)) {				\
				ret = -1; /* callered checked empty, so must be invalid */	\
				tval = defval;												\
			} else if ((temp < min.FIELD									\
							&& !(level_flags & PSMI_ENVVAR_FLAG_NOMIN))		\
					|| (temp > max.FIELD									\
							&& !(level_flags & PSMI_ENVVAR_FLAG_NOMAX))) {	\
				ret = -1;													\
				tval = defval;												\
			} else {														\
				tval.FIELD = (TYPE)temp;									\
			}																\
		}																	\
	} while (0)

	switch (type) {
	case PSMI_ENVVAR_TYPE_YESNO:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else {
			switch (psm3_parse_str_yesno(env, &tval.e_int)) {
			case -1:	// empty, use default
				psmi_assert(0);	// shouldn't happen, checked for empty above
				tval = defval;
				ret = 1;
				break;
			case -2:	// bad syntax, use default
				tval = defval;
				ret = -1;
				break;
			default:	// valid input
				_GETENV_CHECK(tval);
				break;
			}
		}
		_GETENV_PRINT(env, ret, "%s", tval.e_int ? "YES" : "NO",
			      defval.e_int ? "YES" : "NO");
		break;

	case PSMI_ENVVAR_TYPE_STR:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else {
			tval.e_str = env;
			_GETENV_CHECK(tval);
		}
		_GETENV_PRINT(env, ret, "'%s'", tval.e_str, defval.e_str);
		break;

	case PSMI_ENVVAR_TYPE_INT:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else {
			_CONVERT_TO_NUM(e_int,int,signed,INT_MIN,INT_MAX);
			if (ret == 0)
				_GETENV_CHECK(tval);
		}
		FORMAT_RANGESTR(e_int, "%d");
		_GETENV_PRINT(env, ret, "%d", tval.e_int, defval.e_int);
		break;

	case PSMI_ENVVAR_TYPE_UINT:
	case PSMI_ENVVAR_TYPE_UINT_FLAGS:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else {
			_CONVERT_TO_NUM(e_uint,unsigned int,unsigned,0,UINT_MAX);
			if (ret == 0)
				_GETENV_CHECK(tval);
		}
		if (type == PSMI_ENVVAR_TYPE_UINT_FLAGS) {
			FORMAT_RANGESTR(e_uint, "0x%x");
			_GETENV_PRINT(env, ret, "0x%x", tval.e_uint,
				      defval.e_uint);
		} else {
			FORMAT_RANGESTR(e_uint, "%u");
			_GETENV_PRINT(env, ret, "%u", tval.e_uint,
				      defval.e_uint);
		}
		break;

	case PSMI_ENVVAR_TYPE_LONG:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else {
			_CONVERT_TO_NUM(e_long,long,signed,LONG_MIN,LONG_MAX);
			if (ret == 0)
				_GETENV_CHECK(tval);
		}
		FORMAT_RANGESTR(e_long, "%ld");
		_GETENV_PRINT(env, ret, "%ld", tval.e_long, defval.e_long);
		break;
	case PSMI_ENVVAR_TYPE_ULONG_ULONG:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else {
			_CONVERT_TO_NUM(e_ulonglong,unsigned long long,unsigned,0,ULLONG_MAX);
			if (ret == 0)
				_GETENV_CHECK(tval);
		}
		FORMAT_RANGESTR(e_ulonglong, "%llu");
		_GETENV_PRINT(env, ret, "%llu",
			      tval.e_ulonglong, defval.e_ulonglong);
		break;
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_INT:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else if (check) {
			// check will parse_val_pattern_int and check value returned
			// caller must parse again
			tval.e_str = env;
			_GETENV_CHECK(tval);
		} else {
			int trash;
			// we parse just for syntax check, caller must parse again
			switch (psm3_parse_val_pattern_int(env, 0, &trash, level_flags,
				(level_flags & PSMI_ENVVAR_FLAG_NOMIN)?INT_MIN:min.e_int,
				(level_flags & PSMI_ENVVAR_FLAG_NOMAX)?INT_MAX:max.e_int)) {
			case -1:	// empty, use default
				psmi_assert(0);	// shouldn't happen, checked for empty above
				tval = defval;
				ret = 1;
				break;
			case -2:	// one or more fields with bad syntax, use default
				tval = defval;
				ret = -1;
				break;
			default:	// valid string
				tval.e_str = env;
				break;
			}
		}
		FORMAT_RANGESTR(e_int, "%d");
		_GETENV_PRINT(env, ret, "'%s'", tval.e_str, defval.e_str);
		break;
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT_FLAGS:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else if (check) {
			// check will parse_val_pattern_uint and check value returned
			// caller must parse again
			tval.e_str = env;
			_GETENV_CHECK(tval);
		} else {
			unsigned trash;
			// we parse just for syntax check, caller must parse again
			switch (psm3_parse_val_pattern_uint(env, 0, &trash, level_flags,
				(level_flags & PSMI_ENVVAR_FLAG_NOMIN)?0:min.e_uint,
				(level_flags & PSMI_ENVVAR_FLAG_NOMAX)?UINT_MAX:max.e_uint)) {
			case -1:	// empty, use default
				psmi_assert(0);	// shouldn't happen, checked for empty above
				tval = defval;
				ret = 1;
				break;
			case -2:	// one or more fields with bad syntax, use default
				tval = defval;
				ret = -1;
				break;
			default:	// valid string
				tval.e_str = env;
				break;
			}
		}
		if (type == PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT_FLAGS)
			FORMAT_RANGESTR(e_uint, "0x%x");
		else
			FORMAT_RANGESTR(e_uint, "%u");
		_GETENV_PRINT(env, ret, "'%s'", tval.e_str, defval.e_str);
		break;
	case PSMI_ENVVAR_TYPE_STR_TUPLES:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else if (check) {
			// check will parse_str_tuples and check their values
			// caller must parse again
			tval.e_str = env;
			_GETENV_CHECK(tval);
		} else {
			// we parse just for syntax check, caller must parse again
			int vals[3];
			int ntup = psm3_count_tuples(defval.e_str);
			psmi_assert_always(ntup > 0 && ntup <= 3);
			switch (psm3_parse_str_tuples(env, ntup, vals)) {
			case -1:	// empty, use default
				psmi_assert(0);	// shouldn't happen, checked for empty above
				tval = defval;
				ret = 1;
				break;
			case -2:	// one or more fields with bad syntax, use default
				tval = defval;
				ret = -1;
				break;
			default:	// valid string
				tval.e_str = env;
				break;
			}
		}
		_GETENV_PRINT(env, ret, "'%s'", tval.e_str, defval.e_str);
		break;
	case PSMI_ENVVAR_TYPE_ULONG:
	case PSMI_ENVVAR_TYPE_ULONG_FLAGS:
	default:
		if (!env || *env == '\0') {
			tval = defval;
			ret = 1;
		} else {
			_CONVERT_TO_NUM(e_ulong,unsigned long,unsigned,0,ULONG_MAX);
			if (ret == 0)
				_GETENV_CHECK(tval);
		}
		if (type == PSMI_ENVVAR_TYPE_ULONG_FLAGS) {
			FORMAT_RANGESTR(e_ulong, "0x%lx");
			_GETENV_PRINT(env, ret, "0x%lx", tval.e_ulong,
				      defval.e_ulong);
		} else {
			FORMAT_RANGESTR(e_ulong, "%lu");
			_GETENV_PRINT(env, ret, "%lu", tval.e_ulong,
				      defval.e_ulong);
		}
		break;
	}
#undef FORMAT_RANGESTR
#undef _GETENV_CHECK
#undef _GETENV_PRINT
#undef _CONVERT_TO_NUM
	*newval = tval;
	switch (ret) {
	case 0:	// good input
		psm3_stats_print_env(name, env);
		break;
	case -1: // bad input, used default
		// _GETENV_PRINT has set staterrstr
		psm3_stats_print_env(name, statserrstr);
		if (level_flags & PSMI_ENVVAR_FLAG_FATAL) {
			// treat syntax or invalid input as fatal
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_PARAM_ERR,
				"Invalid value for %s: '%s', can't proceed\n",
				name, env);
		}
		break;
	case 1: // no input, used default
		// nothing special here
		// as needed psm3_stats_initialize will log the stats controls
		break;
	}
	return ret;
}
MOCK_DEF_EPILOGUE(psm3_getenv_range);

int
MOCKABLE(psm3_getenv)(const char *name, const char *descr,  int level,
	    int type, union psmi_envvar_val defval,
	    union psmi_envvar_val *newval)
{
	switch (type) {
	case PSMI_ENVVAR_TYPE_YESNO:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)0, (union psmi_envvar_val)1, NULL, NULL, newval);
		break;

	case PSMI_ENVVAR_TYPE_STR:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL, NULL, NULL, newval);
		break;

	case PSMI_ENVVAR_TYPE_INT:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)INT_MIN, (union psmi_envvar_val)INT_MAX, NULL, NULL, newval);
		break;

	case PSMI_ENVVAR_TYPE_UINT:
	case PSMI_ENVVAR_TYPE_UINT_FLAGS:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)0, (union psmi_envvar_val)UINT_MAX, NULL, NULL, newval);
		break;

	case PSMI_ENVVAR_TYPE_LONG:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)LONG_MIN, (union psmi_envvar_val)LONG_MAX, NULL, NULL, newval);
		break;
	case PSMI_ENVVAR_TYPE_ULONG_ULONG:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)0, (union psmi_envvar_val)ULLONG_MAX, NULL, NULL, newval);
		break;
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_INT:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT:
	case PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT_FLAGS:
	case PSMI_ENVVAR_TYPE_STR_TUPLES:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL, NULL, NULL, newval);
		break;
	case PSMI_ENVVAR_TYPE_ULONG:
	case PSMI_ENVVAR_TYPE_ULONG_FLAGS:
	default:
		return psm3_getenv_range(name, descr, NULL, (unsigned)level|PSMI_ENVVAR_FLAG_NOMIN_NOMAX, type, defval, (union psmi_envvar_val)0, (union psmi_envvar_val)ULONG_MAX, NULL, NULL, newval);
		break;
	}
}
MOCK_DEF_EPILOGUE(psm3_getenv);

/*
 * Parsing int parameters
 * 0 -> ok, *val updated
 * -1 -> empty string, *val not updated
 * -2 -> parse error, *val not updated
 */
int psm3_parse_str_int(const char *string, int *val, int min, int max)
{
	int ret;
	long long temp;

	if ((ret = convert_str_signed(string, &temp, min, max)))
		return ret;
	*val = (int)temp;
	return 0;
}

/*
 * Parsing uint parameters
 * 0 -> ok, *val updated
 * -1 -> empty string, *val not updated
 * -2 -> parse error, *val not updated
 */
int psm3_parse_str_uint(const char *string, unsigned int *val,
						unsigned int min, unsigned int max)
{
	int ret;
	unsigned long long temp;

	if ((ret = convert_str_unsigned(string, &temp, min, max)))
		return ret;
	*val = (unsigned int)temp;
	return 0;
}

/*
 * Parsing long parameters
 * Returns:
 * 0 -> ok, *val updated
 * -1 -> empty string, *val not updated
 * -2 -> parse error, *val not updated
 */
int psm3_parse_str_long(const char *string, long *val, long min, long max)
{
	int ret;
	long long temp;

	if ((ret = convert_str_signed(string, &temp, min, max)))
		return ret;
	*val = (long)temp;
	return 0;
}

/*
 * Parsing yesno parameters
 * allows: yes/no, true/false, on/off, 1/0
 * Returns:
 * 0 -> ok, *val updated
 *		*val = 0 - no selected
 *		*val = 1 - yes selected
 * -1 -> empty string, *val not updated
 * -2 -> parse error, *val not updated
 */
int psm3_parse_str_yesno(const char *string, int *val)
{
	psmi_assert(val != NULL);
	if (! string || ! *string)
		return -1;
	else if (string[0] == 'Y' || string[0] == 'y'
				|| string[0] == 'T' || string[0] == 't'
				|| ((string[0] == 'O' || string[0] == 'o')
					&& (string[1] == 'n' || string[1] == 'N'))) {
		*val = 1;
	} else if (string[0] == 'N' || string[0] == 'n'
				|| string[0] == 'F' || string[0] == 'f'
				|| ((string[0] == 'O' || string[0] == 'o')
					&& (string[1] == 'f' || string[1] == 'F'))) {
		*val = 0;
	} else {
		unsigned long long temp;
		if (convert_str_unsigned(string, &temp, 0, UINT_MAX))
			return -2;	// already checked for empty, so must be invalid value
		*val = (temp != 0);
	}
	return 0;
}

/* parse int env of the form 'val' or 'val:' or 'val:pattern'
 * for PSM3_PRINT_STATS
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
							unsigned flags, int min, int max)
{
	int ret = 0;
	long long temp;

	psmi_assert(val != NULL);
	if (!env || ! *env) {
		*val = def;
		ret = -1;
	} else {
		char *e = psmi_strdup(NULL, env);
		char *p;

		if (flags & PSMI_ENVVAR_FLAG_NOMIN)
			min = INT_MIN;
		if (flags & PSMI_ENVVAR_FLAG_NOMAX)
			max = INT_MAX;

		psmi_assert_always(e != NULL);
		if (e == NULL) { // for klocwork
			*val = def;
			goto done;
		}
		p = strchr(e, ':');
		if (p)
			*p = '\0';
		if (!(flags & (PSMI_ENVVAR_FLAG_NOMIN|PSMI_ENVVAR_FLAG_NOABBREV))
				&& (!strcasecmp(e, "min") || !strcasecmp(e, "minimum")))
			*val = min;
		else if (!(flags & (PSMI_ENVVAR_FLAG_NOMAX|PSMI_ENVVAR_FLAG_NOABBREV))
				&& (!strcasecmp(e, "max") || !strcasecmp(e, "maximum")))
			*val = max;
		else if (convert_str_signed(e, &temp, min, max)) {
			*val = def;
			ret = -2;
		} else {
			*val = (int)temp;
		}
		if (ret == 0 && p) {
			if (! *(p+1)) { // val: -> val:*:rank0
				if (psm3_get_myrank() != 0)
					*val = def;
#ifdef FNM_EXTMATCH
			} else if (0 != fnmatch(p+1, psm3_get_mylabel(),  FNM_EXTMATCH )) {
#else
			} else if (0 != fnmatch(p+1, psm3_get_mylabel(),  0 )) {
#endif
				*val = def;
			}
		}
		psmi_free(e);
	}
done:
	return ret;
}

/* parse unsigned env of the form 'val' or 'val:' or 'val:pattern'
 * for PSM3_VERBOSE_ENV, PSM3_TRACEMASK, PSM3_FI and PSM3_IDENITFY
 * Returns:
 * 0 - parsed and matches current process, *val set to parsed val
 * 0 - parsed and doesn't match current process, *val set to def
 * -1 - nothing provided, *val set to def
 * -2 - syntax error, *val set to def
 * flags PSMI_ENVVAR_FLAG_NOMIN, PSMI_ENVVAR_FLAG_NOMAX and
 * PSMI_ENVVAR_FLAG_NOABBREV control if 'min', 'minimum', 'max' or 'maximum'
 * allowed as input and indicate if min and/or max supplied.
 */
int psm3_parse_val_pattern_uint(const char *env, unsigned def, unsigned *val,
								unsigned flags, unsigned min, unsigned max)
{
	int ret = 0;
	unsigned long long temp;

	psmi_assert(val != NULL);
	if (!env || ! *env) {
		*val = def;
		ret = -1;
	} else {
		char *e = psmi_strdup(NULL, env);
		char *p;

		if (flags & PSMI_ENVVAR_FLAG_NOMIN)
			min = 0;
		if (flags & PSMI_ENVVAR_FLAG_NOMAX)
			max = UINT_MAX;

		psmi_assert_always(e != NULL);
		if (e == NULL) { // for klocwork
			*val = def;
			goto done;
		}
		p = strchr(e, ':');
		if (p)
			*p = '\0';
		if (!(flags & (PSMI_ENVVAR_FLAG_NOMIN|PSMI_ENVVAR_FLAG_NOABBREV))
				&& (!strcasecmp(e, "min") || !strcasecmp(e, "minimum")))
			*val = min;
		else if (!(flags & (PSMI_ENVVAR_FLAG_NOMAX|PSMI_ENVVAR_FLAG_NOABBREV))
				&& (!strcasecmp(e, "max") || !strcasecmp(e, "maximum")))
			*val = max;
		else if (convert_str_unsigned(e, &temp, min, max)) {
			*val = def;
			ret = -2;
		} else {
			*val = (unsigned)temp;
		}
		if (ret == 0 && p) {
			if (! *(p+1)) { // val: -> val:*:rank0
				if (psm3_get_myrank() != 0)
					*val = def;
#ifdef FNM_EXTMATCH
			} else if (0 != fnmatch(p+1, psm3_get_mylabel(),  FNM_EXTMATCH )) {
#else
			} else if (0 != fnmatch(p+1, psm3_get_mylabel(),  0 )) {
#endif
				*val = def;
			}
		}
		psmi_free(e);
	}
done:
	return ret;
}

/*
 * Parsing int parameters set in string tuples.
 * Output array int *vals should be able to store 'ntup' elements
 * and should be initialized by caller with default values for each field.
 * Values are only overwritten if they are parsed.
 * Tuples are always separated by colons ':'
 * Empty parameters are left unchanged in vals[]
 * It's valid for less than ntup values to be supplied, any unsupplied
 * fields are not updated in vals[]
 * Returns:
 * 0 - parsed with no errors, vals[] updated
 * -1 - empty or NULL string, vals[] unchanged
 * -2 -  syntax error in one of more of the parameters
 *			parameters with syntax errors are unchanged, others without
 *			syntax errors are updated in vals[]
 */
int psm3_parse_str_tuples(const char *string, int ntup, int *vals)
{
	char *b = (char *)string;
	char *e = b;
	int tup_i = 0;
	int ret = 0;

	psmi_assert(vals != NULL);
	if (! string || ! *string)
		return -1;

	char *buf = psmi_strdup(NULL, string);
	psmi_assert_always(buf != NULL);
	if (! buf)	// for klocwork
		return 0;

	while (*e && tup_i < ntup) {
		b = e;
		while (*e && *e != ':')
			e++;
		if (e > b) {	/* something to parse */
			int len = e - b;
			long long temp;
			strncpy(buf, b, len);
			buf[len] = '\0';
			if (convert_str_signed(buf, &temp, INT_MIN, INT_MAX))
				ret = -2;
			else
				vals[tup_i] = (int)temp;
		}
		if (*e == ':')
			e++;	/* skip delimiter */
		tup_i++;
	}
	if (*e)	// too many tuples
		ret = -2;
	psmi_free(buf);
	return ret;
}

#if defined(PSM_VERBS) || defined(PSM_SOCKETS)
// return forced speed in mbps or 0 if not forced
unsigned long psm3_parse_force_speed()
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned long saved;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (have_value)
		return saved;

	psm3_getenv("PSM3_FORCE_SPEED", "Override for device link speed file in /sys/class.  Specified in mbps. Default is 0 [no override]",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_ULONG,
			(union psmi_envvar_val)0 /* Disabled by default */,
			&envval);
	saved = envval.e_ulong;
	have_value = 1;
	return saved;
}
#endif /* defined(PSM_VERBS) || defined(PSM_SOCKETS) */

// should PSM3 set the cpu affinity itself
int psm3_env_psm_sets_cpuaffinity(int skip_affinity)
{
	// we parse per call in case middleware wants to control per EP
	union psmi_envvar_val envval;

	// algorithm below is equivalent of:
	// PSM3_FORCE_CPUAFFINITY || ! (skip_affinity || PSM3_NO_CPUAFFINITY)

	psm3_getenv("PSM3_FORCE_CPUAFFINITY", "Force PSM3 selection of process CPU affinity.  Default is 0 [not forced]\n",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_YESNO,
			(union psmi_envvar_val)0 /* Disabled by default */, &envval);
	if (envval.e_int)
		return 1;
	if (skip_affinity)
		return 0;

	psm3_getenv("PSM3_NO_CPUAFFINITY", "Skip PSM3 selection of process CPU affinity (overridden by PSM3_FORCE_CPUAFFINITY).  Default is 0 [not skipped]\n",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_YESNO,
			(union psmi_envvar_val)0 /* Disabled by default */,
			&envval);
	if (envval.e_int)
		return 0;
	return 1;
}
