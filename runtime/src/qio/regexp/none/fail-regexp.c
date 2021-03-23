/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
#include "qio_regex.h"
#include "qbuffer.h" // qio_strdup, refcount functions, VOID_PTR_DIFF
#include "error.h"

void qio_regex_init_default_options(qio_regex_options_t* opt)
{
  chpl_internal_error("No Regex Support");
}

// err_str must be freed by the caller 
// returns true if created OK
void qio_regex_create_compile(const char* str, int64_t str_len, const qio_regex_options_t* options, qio_regex_t* compiled)
{
  chpl_internal_error("No Regex Support");
}

void qio_regex_create_compile_flags(const char* str, int64_t str_len, const char* flags, int64_t flags_len, qio_bool isUtf8, qio_regex_t* compiled)
{
  chpl_internal_error("No Regex Support");
}

void qio_regex_retain(const qio_regex_t* compiled)
{
}
void qio_regex_release(qio_regex_t* compiled)
{
}


void qio_regex_get_options(const qio_regex_t* regex, qio_regex_options_t* options)
{
}

void qio_regex_get_pattern(const qio_regex_t* regex, const char** pattern)
{
}

int64_t qio_regex_get_ncaptures(const qio_regex_t* regex)
{
  return 0;
}

qio_bool qio_regex_ok(const qio_regex_t* regex)
{
  return false;
}
const char* qio_regex_error(const qio_regex_t* regex)
{
  return qio_strdup("No Regex Support");
}

bool qio_regex_match(qio_regex_t* regex, const char* text, int64_t str_len, int64_t startpos, int64_t endpos, int anchor, qio_regex_string_piece_t* submatch, int64_t nsubmatch)
{
  chpl_internal_error("No Regex Support");
  return false;
}

int64_t qio_regex_replace(qio_regex_t* regex, const char* repl, int64_t repl_len, const char* str, int64_t str_len, int64_t startpos, int64_t endpos, qio_bool global, const char** str_out, int64_t* len_out)
{
  chpl_internal_error("No Regex Support");
  return 0;
}

qioerr qio_regex_channel_match(const qio_regex_t* regex, const int threadsafe, struct qio_channel_s* ch, int64_t maxlen, int anchor, qio_bool can_discard, qio_bool keep_unmatched, qio_bool keep_whole_pattern, qio_regex_string_piece_t* submatch, int64_t nsubmatch)
{
  chpl_internal_error("No Regex Support");
  return 0;
}

