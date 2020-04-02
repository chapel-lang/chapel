/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "stdchplrt.h"
#include "qio_regexp.h"
#include "qbuffer.h" // qio_strdup, refcount functions, VOID_PTR_DIFF
#include "error.h"

void qio_regexp_init_default_options(qio_regexp_options_t* opt)
{
  chpl_internal_error("No Regexp Support");
}

// err_str must be freed by the caller 
// returns true if created OK
void qio_regexp_create_compile(const char* str, int64_t str_len, const qio_regexp_options_t* options, qio_regexp_t* compiled)
{
  chpl_internal_error("No Regexp Support");
}

void qio_regexp_create_compile_flags(const char* str, int64_t str_len, const char* flags, int64_t flags_len, qio_bool isUtf8, qio_regexp_t* compiled)
{
  chpl_internal_error("No Regexp Support");
}

void qio_regexp_retain(const qio_regexp_t* compiled)
{
}
void qio_regexp_release(qio_regexp_t* compiled)
{
}


void qio_regexp_get_options(const qio_regexp_t* regexp, qio_regexp_options_t* options)
{
}

void qio_regexp_get_pattern(const qio_regexp_t* regexp, const char** pattern)
{
}

int64_t qio_regexp_get_ncaptures(const qio_regexp_t* regexp)
{
  return 0;
}

qio_bool qio_regexp_ok(const qio_regexp_t* regexp)
{
  return false;
}
const char* qio_regexp_error(const qio_regexp_t* regexp)
{
  return qio_strdup("No Regexp Support");
}

bool qio_regexp_match(qio_regexp_t* regexp, const char* text, int64_t str_len, int64_t startpos, int64_t endpos, int anchor, qio_regexp_string_piece_t* submatch, int64_t nsubmatch)
{
  chpl_internal_error("No Regexp Support");
  return false;
}

int64_t qio_regexp_replace(qio_regexp_t* regexp, const char* repl, int64_t repl_len, const char* str, int64_t str_len, int64_t startpos, int64_t endpos, qio_bool global, const char** str_out, int64_t* len_out)
{
  chpl_internal_error("No Regexp Support");
  return 0;
}

qioerr qio_regexp_channel_match(const qio_regexp_t* regexp, const int threadsafe, struct qio_channel_s* ch, int64_t maxlen, int anchor, qio_bool can_discard, qio_bool keep_unmatched, qio_bool keep_whole_pattern, qio_regexp_string_piece_t* submatch, int64_t nsubmatch)
{
  chpl_internal_error("No Regexp Support");
  return 0;
}

