/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _CHPL_REGEXP_H_
#define _CHPL_REGEXP_H_

// forward declare..
struct qio_channel_s;

#include "sys_basic.h"
#include "qio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct qio_regexp_s {
  void* regexp;
} qio_regexp_t;

static inline
qio_regexp_t qio_regexp_null(void)
{
  qio_regexp_t ret;
  ret.regexp = NULL;
  return ret;
}

typedef struct qio_regexp_options_s {
  qio_bool utf8;
  qio_bool posix;
  qio_bool literal;
  qio_bool nocapture;
  qio_bool ignorecase;
  qio_bool multiline;
  qio_bool dotnl;
  qio_bool nongreedy;
} qio_regexp_options_t;

void qio_regexp_init_default_options(qio_regexp_options_t* options);

// Returns true for ok
// and if false return an error string in *err_str that must
// be freed by the caller (and was made with qio_malloc())
void qio_regexp_create_compile(const char* str, int64_t str_len, const qio_regexp_options_t* options, qio_regexp_t* compiled);
void qio_regexp_create_compile_flags(const char* str, int64_t str_len, const char* flags, int64_t flags_len, qio_bool isUtf8, qio_regexp_t* compiled);

static inline 
void qio_regexp_create_compile_flags_2(void* str, int64_t str_len, void* flags, int64_t flags_len, qio_bool isUtf8, qio_regexp_t* compiled) {
  qio_regexp_create_compile_flags((const char*) str, str_len, (const char*) flags, flags_len, isUtf8, compiled);
}

void qio_regexp_retain(const qio_regexp_t* regexp);
void qio_regexp_release(qio_regexp_t* regexp);

void qio_regexp_get_options(const qio_regexp_t* regexp, qio_regexp_options_t* options);

void qio_regexp_get_pattern(const qio_regexp_t* regexp, const char** pattern);

int64_t qio_regexp_get_ncaptures(const qio_regexp_t* regexp);
qio_bool qio_regexp_ok(const qio_regexp_t* regexp);
const char* qio_regexp_error(const qio_regexp_t* regexp);

enum {
  QIO_REGEXP_ANCHOR_UNANCHORED = 0,
  QIO_REGEXP_ANCHOR_START,
  QIO_REGEXP_ANCHOR_BOTH
};


// These point to locations in the searched string.
typedef struct qio_regexp_string_piece_s {
  int64_t offset; // counting from 0; -1 means "NULL"
  int64_t len;
} qio_regexp_string_piece_t;

static inline qio_bool
qio_regexp_string_piece_isnull(qio_regexp_string_piece_t* sp)
{
  return sp->offset == -1;
}

// find locations of matches
// extract captured expressions
// find and replace
// (maybe match full string, maybe find in middle)
// (maybe we want to do it incrementally)

// full match == consume and check that we stopped at end of string
// after one incremental search

// Using a compiled regular expression, match against str
// if find is set, the pattern may match something after the start
//  of str; if it is not, the pattern must match at the beginning of str
// Returns in matches[0], if nmatches>=1, the match for the entire regexp
// Returns in matches[1], if nmatches>=2, the match for the first
//  capture group in the regexp
// ...
//
// Can be called in a loop once str's start position
// is incremented to be the same as
//  match_start
//
// Also, to do a full match, would need start with find=false
//  and then check that the match_len == length of string.
//
// If it failed to find a match, match_len is set to 0 and match_start
//  is set to NULL.
//
// Returns true if we matched.
qio_bool qio_regexp_match(qio_regexp_t* regexp, const char* str, int64_t str_len, int64_t startpos, int64_t endpos, int anchor, qio_regexp_string_piece_t* submatch, int64_t nsubmatch);

int64_t qio_regexp_replace(qio_regexp_t* regexp, const char* repl, int64_t repl_len, const char* str, int64_t str_len, int64_t startpos, int64_t endpos, qio_bool global, const char** str_out, int64_t* len_out);


// Returns ENOERR if we matched, EFORMAT if we did not, or an IO error.
// Must have a mark already set.
// If can_discard is set,  we revert/advance/mark to 'discard'.
// After the match, updates the channel position.
//  - if there was no match and keep_unmatched is set, put it where we started
//  - if there was no match and keep_unmatched is not set, put it at the
//     end of the channel
//  - if there was a match, put it after the match
//  - if there was an error, we do not adjust the channel position afterwards
//
qioerr qio_regexp_channel_match(const qio_regexp_t* regexp, const int threadsafe, struct qio_channel_s* ch, int64_t maxlen, int anchor, qio_bool can_discard, qio_bool keep_unmatched, qio_bool keep_whole_pattern, qio_regexp_string_piece_t* submatch, int64_t nsubmatch);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
