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

// Make sure that we get the RE2 extensions for Chapel
#ifndef CHPL_RE2
#define CHPL_RE2
#endif

#include <limits>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// We have run into issues when using our chpl-atomics.h file from C++ code
// under CHPL_ATOMICS=cstdlib. As a workaround, just use std::atomic and avoid
// bringing in the Chapel implementation. We know this is safe in this case
// because re2 itself requires std::atomics, and won't build without them.
#define QIO_USE_STD_ATOMICS_REF_CNT 1

#ifndef CHPL_RT_UNIT_TEST
#include "stdchplrt.h"
#endif
#include "qio_regexp.h"
#include "qbuffer.h" // qio_strdup, refcount functions, qio_ptr_diff, etc
#include "qio.h" // for channel operations
#undef printf

#include "re2/re2.h"

using namespace re2;

struct re_cache;

struct re_t {
  RE2 re;
  qbytes_refcnt_t ref_cnt;
  // RE2 implementations are shared and ref-counted.
  // We free the internal RE2 once ref_cnt==0.
  re_t(StringPiece& pattern, const RE2::Options& option, re_cache* home)
    : re(pattern, option)
  {
    // Initialize the reference count to 1.
    // This represents the reference held by the cache.
    DO_INIT_REFCNT(this);
  }
};

// A very simple 8-element local regexp cache.
#define REGEXP_CACHE_SIZE 8
struct cache_elem {
  int64_t date;
  re_t* re;
};

struct re_cache {
  int64_t date;
  cache_elem elems[REGEXP_CACHE_SIZE];
};
static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void local_cache_destroy_elts(re_cache* c);

static
void destroy_key(void* ptr)
{
  re_cache* c = (re_cache*) ptr;
  local_cache_destroy_elts(c);
  qio_free(c);
}

static
void make_key(void)
{
  (void) pthread_key_create(&key, &destroy_key);
}


static inline
re_cache* local_cache(void)
{
  re_cache* ptr;
  (void) pthread_once(&key_once, make_key);
  if((ptr = (re_cache*) pthread_getspecific(key)) == NULL) {
    ptr = (re_cache*) qio_malloc(sizeof(re_cache));
    memset(ptr, 0, sizeof(re_cache));
    (void) pthread_setspecific(key, ptr);
  }
  return ptr;
}

static
void qio_re_options_to_re2_options(const qio_regexp_options_t* options, RE2::Options *opts)
{
  opts->set_utf8(options->utf8);
  opts->set_posix_syntax(options->posix);
  opts->set_literal(options->literal);
  opts->set_never_capture(options->nocapture);
  opts->set_case_sensitive(!options->ignorecase);
  opts->set_one_line(!options->multiline);
  opts->set_dot_nl(options->dotnl);
  opts->set_longest_match(!options->nongreedy);
}

static
void re2_options_to_qio_re_options(const RE2::Options *opts, qio_regexp_options_t* options)
{
  options->utf8 = opts->utf8();
  options->posix = opts->posix_syntax();
  options->literal = opts->literal();
  options->nocapture = opts->never_capture();
  options->ignorecase = !opts->case_sensitive();
  options->multiline = !opts->one_line();
  options->dotnl = opts->dot_nl();
  options->nongreedy = ! opts->longest_match();
}

static
bool equal_options(const RE2::Options *opts, const qio_regexp_options_t* options)
{
  return  options->utf8 == opts->utf8() &&
          options->posix == opts->posix_syntax() &&
          options->literal == opts->literal() &&
          options->nocapture == opts->never_capture() &&
          options->ignorecase == !opts->case_sensitive() &&
          options->multiline == !opts->one_line() &&
          options->dotnl == opts->dot_nl() &&
          options->nongreedy == ! opts->longest_match();
}

static
void re_free(re_t* re)
{
  //fprintf(stdout, "free %p\n", re);
  delete re;
}


static
re_t* local_cache_get(const char* str, int64_t str_len, const qio_regexp_options_t* options) {
  re_cache* c = local_cache();
  int oldest;
  int64_t oldest_date;
  c->date++;
  // Find either the oldest element
  // or a matching element
  oldest = 0;
  oldest_date = c->elems[0].date;
  for( int i = 0; i < REGEXP_CACHE_SIZE; i++ ) {
    if( c->elems[i].date < oldest_date ) {
      oldest = i;
      oldest_date = c->elems[i].date;
    }
    if( ! c->elems[i].re ) continue;
    const string& pat = c->elems[i].re->re.pattern();
    const RE2::Options& opt = c->elems[i].re->re.options();
    if( (uint64_t) pat.length() == (uint64_t) str_len &&
        0 == memcmp(pat.data(), str, str_len ) &&
        equal_options(&opt, options) ) {
      // Make the date current.
      c->elems[i].date = c->date;
      // Return this element.
      re_t* re = c->elems[i].re;
      // We increment the reference count before returning a copy to the
      // caller.  It is up to the caller to release the re_t handle when done.
      DO_RETAIN(re);
      return re;
    }
  }

  // If we found no match, replace oldest.
  if( c->elems[oldest].re) DO_RELEASE(c->elems[oldest].re, re_free);

  // Put a new RE in that slot.
  RE2::Options opts;
  qio_re_options_to_re2_options(options, &opts);
  StringPiece strp(str, str_len);
  re_t* re = new re_t(strp, opts, c);
  c->elems[oldest].date = c->date;
  c->elems[oldest].re = re;
  // We increment the reference count before returning a copy to the
  // caller.  It is up to the caller to release the re_t handle when done.
  DO_RETAIN(re);
  return re;
}

static
void local_cache_destroy_elts(re_cache* c) {
  if (c) {
    // Destroy all of the elements in the local cache.
    for( int i = 0; i < REGEXP_CACHE_SIZE; i++ ) {
      re_t* re = c->elems[i].re;
      if (re) {
        DO_RELEASE(re, re_free);
      }
    }
  }
}




void qio_regexp_init_default_options(qio_regexp_options_t* opt)
{
  opt->utf8 = true;
  opt->posix = false;
  opt->literal = false;
  opt->nocapture = false;
  opt->ignorecase = false;
  opt->multiline = false;
  opt->dotnl = false;
  opt->nongreedy = false;
}

// The returned re_t (passed back through "compiled") must be released by the caller.
void qio_regexp_create_compile(const char* str, int64_t str_len, const qio_regexp_options_t* options, qio_regexp_t* compiled)
{
  re_t* regexp = local_cache_get(str, str_len, options);
  compiled->regexp = (void*) regexp;
  // We bump the reference count, because caller "owns" its copy of the cached
  // regexp.  This way, a regexp can be removed from the cache without causing a
  // copy that is still in use to be deleted early.
  DO_RETAIN(regexp);
}

// The re_t returned in compiled must be released by the caller.
void qio_regexp_create_compile_flags(const char* str, int64_t str_len, const char* flags, int64_t flags_len, qio_bool isUtf8, qio_regexp_t* compiled)
{
  qio_regexp_options_t opt;
  qio_regexp_init_default_options(&opt);
  // Parse the flags into options.
  for(int i = 0; i < flags_len; i++ ) {
    if( flags[i] == 'i' ) opt.ignorecase = true;
    if( flags[i] == 'm' ) opt.multiline = true;
    if( flags[i] == 's' ) opt.dotnl = true;
    if( flags[i] == 'U' ) opt.nongreedy = true;
  }
  opt.utf8 = isUtf8; 

  return qio_regexp_create_compile(str, str_len, &opt, compiled);
}

void qio_regexp_retain(const qio_regexp_t* compiled)
{
  re_t* re = (re_t*) compiled->regexp;
  //fprintf(stdout, "Retain %p\n", re);
  DO_RETAIN(re);
}

void qio_regexp_release(qio_regexp_t* compiled)
{
  re_t* re = (re_t*) compiled->regexp;
  //fprintf(stdout, "Release %p\n", re);
  DO_RELEASE(re, re_free);
  compiled->regexp = NULL;
}

void qio_regexp_get_options(const qio_regexp_t* regexp, qio_regexp_options_t* options)
{
  RE2* re2 = (RE2*) regexp->regexp;
  const RE2::Options& opts = re2->options();
  re2_options_to_qio_re_options(&opts, options);
}

void qio_regexp_get_pattern(const qio_regexp_t* regexp, const char** pattern)
{
  RE2* re2 = (RE2*) regexp->regexp;
  *pattern = qio_strdup(re2->pattern().c_str());
}

int64_t qio_regexp_get_ncaptures(const qio_regexp_t* regexp)
{
  RE2* re2 = (RE2*) regexp->regexp;
  return re2->NumberOfCapturingGroups();
}
qio_bool qio_regexp_ok(const qio_regexp_t* regexp)
{
  RE2* re2 = (RE2*) regexp->regexp;
  return re2->ok();
}

const char* qio_regexp_error(const qio_regexp_t* regexp)
{
  RE2* re2 = (RE2*) regexp->regexp;
  return qio_strdup(re2->error().c_str());
}

qio_bool qio_regexp_match(qio_regexp_t* regexp, const char* text, int64_t text_len, int64_t startpos, int64_t endpos, int anchor, qio_regexp_string_piece_t* submatch, int64_t nsubmatch)
{
  StringPiece textp(text, text_len);
  bool ret;
  RE2::Anchor ranchor = RE2::UNANCHORED;
  MAYBE_STACK_SPACE(StringPiece, onstack);
  StringPiece* spPtr;
  RE2* re = (RE2*) regexp->regexp;

  // RE2 uses int for ncaptures
  if( nsubmatch > INT_MAX || nsubmatch < 0 )
    return false;

  if( anchor == QIO_REGEXP_ANCHOR_UNANCHORED ) ranchor = RE2::UNANCHORED;
  else if( anchor == QIO_REGEXP_ANCHOR_START ) ranchor = RE2::ANCHOR_START;
  else if( anchor == QIO_REGEXP_ANCHOR_BOTH ) ranchor = RE2::ANCHOR_BOTH;

  MAYBE_STACK_ALLOC(StringPiece, nsubmatch, spPtr, onstack);
  memset((void*)spPtr, 0, sizeof(StringPiece)*nsubmatch);

  ret = re->Match(textp, startpos, endpos, ranchor, spPtr, nsubmatch);
  // Now set submatch based on StringPieces
  for( int64_t i = 0; i < nsubmatch; i++ ) {
    if( !ret || spPtr[i].data() == NULL ) {
      submatch[i].offset = -1;
      submatch[i].len = 0;
    } else {
      intptr_t diff = 0;
      if( ! spPtr[i].empty() ) {
        diff = qio_ptr_diff((void*) spPtr[i].data(), (void*) textp.data());
        assert( diff >= 0 && diff <= endpos );
      }
      submatch[i].offset = diff;
      submatch[i].len = spPtr[i].length();
    }
  }

  MAYBE_STACK_FREE(spPtr, onstack);

  return ret;
}

int64_t qio_regexp_replace(qio_regexp_t* regexp, const char* repl, int64_t repl_len, const char* str, int64_t str_len, int64_t startpos, int64_t endpos, qio_bool global, const char** str_out, int64_t* len_out)
{
  // This could make fewer copies of everything...
  // ... but it will work for the moment and this is the
  //     expedient way.
  StringPiece rewrite(repl, repl_len);
  std::string s(str, str_len);
  RE2* re = (RE2*) regexp->regexp;
  int64_t ret = 0;
  char* output = NULL;
  if( global ) {
    ret = RE2::GlobalReplace(&s, *re, rewrite);
  } else {
    if( RE2::Replace(&s, *re, rewrite) ) {
      ret = 1;
    } else {
      ret = 0;
    }
  }
  output = (char*) qio_malloc(s.length()+1);
  memcpy(output, s.data(), s.length());
  output[s.length()] = '\0';
  *str_out = output;
  *len_out = s.length();
  return ret;
}

int qio_regexp_channel_read_byte(qio_channel_s* ch);
void qio_regexp_channel_discard(qio_channel_s* ch, int64_t cur, int64_t min);

int qio_regexp_channel_read_byte(qio_channel_s* ch)
{
  int ret;
  ret = qio_channel_read_byte(false, ch);
  //printf("READ BYTE %i %c off=%i\n", ret, ret, (int)qio_channel_offset_unlocked(ch));
  return ret;
}

void qio_regexp_channel_discard(qio_channel_s* ch, int64_t cur, int64_t min)
{
  int64_t buf;
  int64_t off;

  off = qio_channel_offset_unlocked(ch);

  //printf("DISCARD CALLED: cur=%i min=%i off=%i\n",(int)cur,(int)min,(int)off);
  qio_channel_revert_unlocked(ch);
  buf = qio_channel_offset_unlocked(ch);

  if( min > off ) min = off;
  if( min < buf ) min = buf;

  // advance to target.
  //printf("DISCARD CALLED: advance to %i\n", (int) target);
  qio_channel_advance_unlocked(ch, min - buf);
  //printf("DISCARD CALLED: mark\n");
  qio_channel_mark(false, ch);
  // advance to off.
  //printf("DISCARD CALLED: advance to %i\n", (int) off);
  qio_channel_advance_unlocked(ch, off - min);

  // error checking.
  assert( qio_channel_offset_unlocked(ch) == off );
}


qioerr qio_regexp_channel_match(const qio_regexp_t* regexp, const int threadsafe, struct qio_channel_s* ch, int64_t maxlen, int anchor, qio_bool can_discard, qio_bool keep_unmatched, qio_bool keep_whole_pattern, qio_regexp_string_piece_t* captures, int64_t ncaptures)
{
  RE2* re = (RE2*) regexp->regexp;
  qioerr err;
  void* bufstart = NULL;
  void* bufend = NULL;
  RE2::Anchor ranchor = RE2::UNANCHORED;
  int64_t need;
  int64_t start_offset, offset, end_offset; 
  int64_t end;
  int64_t match_start = -1;
  int64_t match_len = 0;
  StringPiece buffer;
  FilePiece text;
  FilePiece* locs;
  FileSearchInfo ci;
  bool found = false;
  int i;
  int use_captures = ncaptures;
  MAYBE_STACK_SPACE(FilePiece, caps_onstack);

  if( ncaptures > INT_MAX || ncaptures < 0 )
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "invalid number of captures");

  start_offset = offset = qio_channel_offset_unlocked(ch);
  end_offset = qio_channel_end_offset_unlocked(ch);

  end = end_offset;
  if( maxlen != std::numeric_limits<int64_t>::max() ) {
    if( end_offset - start_offset > maxlen ) end = start_offset + maxlen;
  }

  // Fill in FileSearchInfo
  ci.byte = 0;
  ci.prev_byte = 0;
  ci.offset = start_offset-1;
  ci.end_offset = end_offset;
  ci.file = ch;
  ci.read_byte_fn = (read_byte_fn_t) &qio_regexp_channel_read_byte;
  ci.discard_fn = (discard_fn_t) &qio_regexp_channel_discard;
  ci.re = re;
  if( ncaptures <= 0 ) ci.nmatch = 0;
  else if( ncaptures == 1 ) ci.nmatch = 1;
  else if( ncaptures < INT_MAX ) ci.nmatch = (int) ncaptures;
  else ci.nmatch = INT_MAX;
  ci.can_discard = can_discard;
  ci.keep_unmatched = keep_unmatched;
  ci.keep_whole_pattern = keep_whole_pattern;

  if( anchor == QIO_REGEXP_ANCHOR_UNANCHORED ) ranchor = RE2::UNANCHORED;
  else if( anchor == QIO_REGEXP_ANCHOR_START ) ranchor = RE2::ANCHOR_START;
  else if( anchor == QIO_REGEXP_ANCHOR_BOTH ) ranchor = RE2::ANCHOR_BOTH;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  if( ! qio_channel_has_mark_unlocked(ch) ) {
    // Should always have a mark when calling this function.
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "missing mark");
    goto markerror;
  }

  // Require at least 1 byte and at most 1024 bytes.
  need = re->min_match_length_bytes();
  if( need <= 0 ) need = 1;
  if( need > 1024) need = 1024;
  err = qio_channel_require_read(false, ch, need);
  if( qio_err_to_int(err) == EEOF ) err = 0; // ignore EOF
  if( err ) goto error;

  // Get the current buffer information...
  err = qio_channel_begin_peek_cached(false, ch, &bufstart, &bufend);
  if( qio_err_to_int(err) == EEOF ) err = 0; // ignore EOF
  if( err ) goto error;

  // We never call end_peek_cached. (should be OK since we do unlock)
 
  if( qio_ptr_diff(bufend, bufstart) > maxlen ) {
    bufend = qio_ptr_add(bufstart, maxlen);
  }

  // Construct the StringPiece for the buffer.
  buffer.set((const char*) bufstart, qio_ptr_diff(bufend, bufstart));
  // and the qio_channel_string_piece
  text.set_channel_info(&ci, start_offset, end);

  if( ncaptures == 0 ) use_captures = 1;
  MAYBE_STACK_ALLOC(FilePiece, use_captures, locs, caps_onstack);
  memset((void*)locs, 0, sizeof(FilePiece) * use_captures);

  found = re->MatchFile(text, buffer, ranchor, locs, ncaptures);

  // Copy capture groups.
  for( i = 0; i < ncaptures; i++ ) {
    if( !found || locs[i].start == -1 ) {
      captures[i].offset = -1;
      captures[i].len = 0;
    } else {
      captures[i].offset = locs[i].start;
      captures[i].len = locs[i].size();
    }
  }

  // Grab the entire match for use below.
  match_start = locs[0].start;
  match_len = locs[0].size();

  MAYBE_STACK_FREE(locs, caps_onstack);

error:

  // Get channel errors from within MatchSpecial1/qio_channel_read_byte 
  if( qio_err_to_int(err) == EEOF ) err = 0; // ignore EOF
  if( ! err ) err = qio_channel_error(ch);
  if( qio_err_to_int(err) == EEOF ) err = 0; // ignore EOF

  end_offset = qio_channel_offset_unlocked(ch);
  qio_channel_revert_unlocked(ch);
  offset = qio_channel_offset_unlocked(ch);
  qio_channel_mark(false, ch);

  // Adjust the channel position
  if( found ) {
    // If there was a match, put it after the match.
    end_offset = match_start + match_len;
    qio_channel_advance(false, ch, end_offset - offset);
  } else {
    if( keep_unmatched ) {
      qio_channel_advance(false, ch, start_offset - offset);
    } else {
      qio_channel_advance(false, ch, end_offset - offset);
    }
  }

markerror:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  if( err == 0 && ! found ) QIO_GET_CONSTANT_ERROR(err, EFORMAT, "no match");

  return err;
}


