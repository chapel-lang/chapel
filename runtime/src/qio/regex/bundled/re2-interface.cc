/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include <limits>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>

// We have run into issues when using our chpl-atomics.h file from C++ code
// under CHPL_ATOMICS=cstdlib. As a workaround, just use std::atomic and avoid
// bringing in the Chapel implementation. We know this is safe in this case
// because re2 itself requires std::atomics, and won't build without them.
#define QIO_USE_STD_ATOMICS_REF_CNT 1

#ifndef CHPL_RT_UNIT_TEST
#include "stdchplrt.h"
#endif
#include "qio_regex.h"
#include "qbuffer.h" // qio_strdup, refcount functions, qio_ptr_diff, etc
#include "qio.h" // for channel operations
#include "chpltypes.h" // must be before "error.h" to prevent include errors
#include "error.h"
#include "encoding/encoding-support.h"
#undef printf

#include "re2/re2.h"

using namespace re2;

struct re_cache;

enum {
  // these ones are handled by RE2
  OPTION_FLAG_UTF8 = 1,
  OPTION_FLAG_POSIX = 2,
  OPTION_FLAG_LITERAL = 4,
  OPTION_FLAG_NOCAPTURE = 8,
  OPTION_FLAG_IGNORECASE = 16,
  OPTION_FLAG_DOTNL = 32,
  // these ones are supported by a prefix
  OPTION_FLAG_MULTILINE = 64, // (?m)
  OPTION_FLAG_NONGREEDY = 128, // (?U)
  // note: if any more are added, 'optionFlags' will need > 1 byte.
};
typedef uint8_t optionFlags_t;
typedef uint8_t optionPrefixLen_t;

static RE2::Options flags_to_re2_opts(optionFlags_t flags);

struct re_t {
  RE2 re2;
  qbytes_refcnt_t ref_cnt;
  // which options?
  optionFlags_t optionFlags;
  // how many bytes of the pattern were automatically added to support options?
  optionPrefixLen_t optionPrefixBytes;

  // RE2 implementations are shared and ref-counted.
  // We free the internal RE2 once ref_cnt==0.
  re_t(StringPiece& pattern,
       optionFlags_t optionFlags,
       optionPrefixLen_t optionPrefixBytes)
    : re2(pattern, flags_to_re2_opts(optionFlags)),
      optionFlags(optionFlags),
      optionPrefixBytes(optionPrefixBytes)
  {
    // Initialize the reference count to 1.
    // This represents the reference held by the cache.
    DO_INIT_REFCNT(this);
  }
};

// A very simple 8-element local regex cache.
#define REGEX_CACHE_SIZE 8
struct cache_elem {
  int64_t date;
  re_t* re;
};

struct re_cache {
  int64_t date;
  cache_elem elems[REGEX_CACHE_SIZE];
  ~re_cache();
};

static RE2::Options flags_to_re2_opts(optionFlags_t flags) {
  RE2::Options opts;
  RE2::Options::Encoding utf8E = RE2::Options::Encoding::EncodingUTF8;
  RE2::Options::Encoding byteE = RE2::Options::Encoding::EncodingLatin1;
  bool utf8 = (flags & OPTION_FLAG_UTF8) != 0;
  bool posix = (flags & OPTION_FLAG_POSIX) != 0;
  bool literal = (flags & OPTION_FLAG_LITERAL) != 0;
  bool nocapture = (flags & OPTION_FLAG_NOCAPTURE) != 0;
  bool ignorecase = (flags & OPTION_FLAG_IGNORECASE) != 0;
  bool dotnl = (flags & OPTION_FLAG_DOTNL) != 0;
  bool multiline = (flags & OPTION_FLAG_MULTILINE) != 0;

  opts.set_encoding(utf8 ? utf8E : byteE);
  opts.set_posix_syntax(posix);
  if (posix) opts.set_longest_match(true); // posix defaults to longest match
  opts.set_literal(literal);
  opts.set_never_capture(nocapture);
  opts.set_case_sensitive(!ignorecase);
  opts.set_one_line(!multiline); // note: only consulted with posix==true
  opts.set_dot_nl(dotnl);

  opts.set_log_errors(false);

  return opts;
}

static
optionFlags_t qio_re_options_to_flags(const qio_regex_options_t* options) {
  optionFlags_t ret = 0;

  // ignore all other flags if literal is used
  if (options->literal) return OPTION_FLAG_LITERAL;

  if (options->utf8)       ret |= OPTION_FLAG_UTF8;
  if (options->posix)      ret |= OPTION_FLAG_POSIX;
  if (options->nocapture)  ret |= OPTION_FLAG_NOCAPTURE;
  if (options->ignorecase) ret |= OPTION_FLAG_IGNORECASE;
  if (options->multiline)  ret |= OPTION_FLAG_MULTILINE;
  if (options->dotnl)      ret |= OPTION_FLAG_DOTNL;
  if (options->nongreedy)  ret |= OPTION_FLAG_NONGREEDY;
  return ret;
}

static
void flags_to_qio_re_options(optionFlags_t flags, qio_regex_options_t* opts) {
  bool utf8 = (flags & OPTION_FLAG_UTF8) != 0;
  bool posix = (flags & OPTION_FLAG_POSIX) != 0;
  bool literal = (flags & OPTION_FLAG_LITERAL) != 0;
  bool nocapture = (flags & OPTION_FLAG_NOCAPTURE) != 0;
  bool ignorecase = (flags & OPTION_FLAG_IGNORECASE) != 0;
  bool dotnl = (flags & OPTION_FLAG_DOTNL) != 0;
  bool multiline = (flags & OPTION_FLAG_MULTILINE) != 0;
  bool nongreedy = (flags & OPTION_FLAG_NONGREEDY) != 0;

  qio_regex_init_default_options(opts);

  opts->utf8 = utf8;
  opts->posix = posix;
  opts->literal = literal;
  opts->nocapture = nocapture;
  opts->ignorecase = ignorecase;
  opts->multiline = multiline;
  opts->dotnl = dotnl;
  opts->nongreedy = nongreedy;
}


static
void re_free(re_t* re)
{
  delete re;
}

static
re_t* local_cache_get(const char* str, int64_t str_len,
                      optionFlags_t optionFlags,
                      optionPrefixLen_t optionPrefixBytes) {
  thread_local re_cache cache;
  re_cache* c = &cache;
  int oldest;
  int64_t oldest_date;
  c->date++;
  // Find either the oldest element
  // or a matching element
  oldest = 0;
  oldest_date = c->elems[0].date;
  for (int i = 0; i < REGEX_CACHE_SIZE; i++) {
    cache_elem* elem = &c->elems[i];
    if( elem->date < oldest_date ) {
      oldest = i;
      oldest_date = elem->date;
    }
    re_t* re = elem->re;
    if (!re) continue;
    const std::string& pat = re->re2.pattern();
    if ((uint64_t) pat.length() == (uint64_t) str_len &&
        re->optionFlags == optionFlags &&
        0 == memcmp(pat.data(), str, str_len)) {
      // Make the date current.
      elem->date = c->date;
      // Return this element.
      // We increment the reference count before returning a copy to the
      // caller.  It is up to the caller to release the re_t handle when done.
      DO_RETAIN(re);
      return re;
    }
  }

  // If we found no match, replace oldest.
  cache_elem* elem = &c->elems[oldest];
  if (elem->re) DO_RELEASE(elem->re, re_free);

  // Put a new RE in that slot.
  StringPiece strp(str, str_len);
  re_t* re = new re_t(strp, optionFlags, optionPrefixBytes);
  elem->date = c->date;
  elem->re = re;
  // We increment the reference count before returning a copy to the
  // caller.  It is up to the caller to release the re_t handle when done.
  DO_RETAIN(re);
  return re;
}

re_cache::~re_cache() {
  // Destroy all of the elements in the local cache.
  for(auto& e : elems) {
    re_t* re = e.re;
    if (re) {
      DO_RELEASE(re, re_free);
    }
  }
}

void qio_regex_init_default_options(qio_regex_options_t* opt)
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

// The returned re_t (passed back through "compiled")
// must be released by the caller.
void qio_regex_create_compile(const char* str, int64_t str_len,
                              const qio_regex_options_t* options,
                              qio_regex_t* compiled)
{
  re_t* regex = nullptr;
  optionFlags_t optionFlags = qio_re_options_to_flags(options);

  if (!options->posix && !options->literal &&
      (options->multiline || options->nongreedy)) {
    // if it's not POSIX mode, need to add a prefix for multiline/nongreedy
    std::string s;
    if (options->multiline) {
      s += "(?m)";
    }
    if (options->nongreedy) {
      s += "(?U)";
    }
    uint8_t optionPrefixBytes = s.size();
    s.append(str, str_len);

    regex = local_cache_get(s.data(), s.size(),
                            optionFlags, optionPrefixBytes);
  } else {
    // otherwise, use the pattern as-is, no need to add a prefix
    regex = local_cache_get(str, str_len, optionFlags, 0);
  }
  compiled->regex = (void*) regex;
  // We bump the reference count, because caller "owns" its copy of the cached
  // regex.  This way, a regex can be removed from the cache without causing a
  // copy that is still in use to be deleted early.
  DO_RETAIN(regex);
}

// The re_t returned in compiled must be released by the caller.
void qio_regex_create_compile_flags(const char* str, int64_t str_len, const char* flags, int64_t flags_len, qio_bool isUtf8, qio_regex_t* compiled)
{
  qio_regex_options_t opt;
  qio_regex_init_default_options(&opt);
  // Parse the flags into options.
  for(int i = 0; i < flags_len; i++ ) {
    if( flags[i] == 'i' ) opt.ignorecase = true;
    if( flags[i] == 'm' ) opt.multiline = true;
    if( flags[i] == 's' ) opt.dotnl = true;
    if( flags[i] == 'U' ) opt.nongreedy = true;
  }
  opt.utf8 = isUtf8;

  return qio_regex_create_compile(str, str_len, &opt, compiled);
}

void qio_regex_retain(const qio_regex_t* compiled)
{
  re_t* re = (re_t*) compiled->regex;
  DO_RETAIN(re);
}

void qio_regex_release(qio_regex_t* compiled)
{
  re_t* re = (re_t*) compiled->regex;
  DO_RELEASE(re, re_free);
  compiled->regex = NULL;
}

void qio_regex_get_options(const qio_regex_t* regex,
                           qio_regex_options_t* options)
{
  if (regex && regex->regex) {
    re_t* re = (re_t*) regex->regex;
    flags_to_qio_re_options(re->optionFlags, options);
  } else {
    qio_regex_init_default_options(options);
  }
}

void qio_regex_borrow_pattern(const qio_regex_t* regex,
                              const char** pattern, int64_t* len_out)
{
  if (regex && regex->regex) {
    re_t* re = (re_t*) regex->regex;
    const std::string &s = re->re2.pattern();
    ssize_t prefixBytes = re->optionPrefixBytes;
    *pattern = s.c_str() + prefixBytes;
    *len_out = s.length() - prefixBytes;
  } else {
    *len_out = 0;
  }
}

int64_t qio_regex_get_ncaptures(const qio_regex_t* regex)
{
  if (regex && regex->regex) {
    re_t* re = (re_t*) regex->regex;
    return re->re2.NumberOfCapturingGroups();
  }
  return 0;
}

qio_bool qio_regex_ok(const qio_regex_t* regex)
{
  if (regex && regex->regex) {
    re_t* re = (re_t*) regex->regex;
    return re->re2.ok();
  }

  return false;
}

const char* qio_regex_error(const qio_regex_t* regex)
{
  if (regex && regex->regex) {
    re_t* re = (re_t*) regex->regex;
    return qio_strdup(re->re2.error().c_str());
  } else {
    return "";
  }
}

qio_bool qio_regex_match(qio_regex_t* regex, const char* text, int64_t text_len, int64_t startpos, int64_t endpos, int anchor, qio_regex_string_piece_t* submatch, int64_t nsubmatch)
{
  StringPiece textp(text, text_len);
  bool ret;
  RE2::Anchor ranchor = RE2::UNANCHORED;
  MAYBE_STACK_SPACE(StringPiece, onstack);
  StringPiece* spPtr;
  re_t* re = (re_t*) regex->regex;
  if (!re) return false;
  RE2* re2 = &re->re2;

  // RE2 uses int for ncaptures
  if( nsubmatch > INT_MAX || nsubmatch < 0 )
    return false;

  if( anchor == QIO_REGEX_ANCHOR_UNANCHORED ) ranchor = RE2::UNANCHORED;
  else if( anchor == QIO_REGEX_ANCHOR_START ) ranchor = RE2::ANCHOR_START;
  else if( anchor == QIO_REGEX_ANCHOR_BOTH ) ranchor = RE2::ANCHOR_BOTH;

  MAYBE_STACK_ALLOC(StringPiece, nsubmatch, spPtr, onstack);
  memset((void*)spPtr, 0, sizeof(StringPiece)*nsubmatch);

  ret = re2->Match(textp, startpos, endpos, ranchor, spPtr, nsubmatch);
  // Now set submatch based on StringPieces
  for( int64_t i = 0; i < nsubmatch; i++ ) {
    if( !ret ) {
      submatch[i].offset = -1;
      submatch[i].len = 0;
    } else {
      intptr_t diff = qio_ptr_diff((void*) spPtr[i].data(), (void*) textp.data());
      if( !(diff == 0 || (diff >= startpos && diff <= endpos)) ) {
        chpl_internal_error("Invalid offset from regex match");
      }
      int64_t length = spPtr[i].length();

      submatch[i].offset = diff;
      submatch[i].len = length;
    }
  }

  MAYBE_STACK_FREE(spPtr, onstack);

  return ret;
}


// returns true for OK
static bool append(char*& buf, // buffer
                   int64_t& buf_sz, // allocated size
                   int64_t& buf_len, // amount used
                   const char* data, // to append
                   int64_t data_len) {
  if (data_len == 0)
    return true; // nothing else to do

  if (data_len < 0)
    return false;

  if (buf == NULL || buf_len + data_len > buf_sz) {
    // reallocate buf
    int64_t new_sz = buf_sz * 2;
    if (new_sz < buf_len + data_len) {
      new_sz = buf_len + data_len + 32;
    }
    if (new_sz < 64) {
      new_sz = 64;
    }
    char* new_buf = (char*) qio_realloc((void*)buf, new_sz);
    if (new_buf == NULL) return false; // failure to allocate
    // record the new buffer and size for the caller
    buf = new_buf;
    buf_sz = new_sz;
  }

  // append the data
  assert(buf_len >= 0 && data_len >= 0 && buf_sz >= 0);
  assert(buf_len + data_len <= buf_sz);
  memcpy(buf + buf_len, data, data_len);
  buf_len += data_len;

  return true; // everything is OK
}

static bool append_rewrite(char*& buf, // buffer
                           int64_t& buf_sz, // allocated size
                           int64_t& buf_len, // amount used
                           const StringPiece& rewrite,
                           const StringPiece* vec,
                           int veclen) {
  for (const char *s = rewrite.data(), *end = s + rewrite.size();
       s < end;
       s++) {
    if (*s != '\\') {
      // append 1 byte
      append(buf, buf_sz, buf_len, s, 1);
      continue;
    }
    s++;
    int c = (s < end) ? *s : EOF;
    if (isdigit(c)) {
      int n = (c - '0');
      if (n >= veclen) {
        return false;
      }
      StringPiece snip = vec[n];
      if (!snip.empty())
        append(buf, buf_sz, buf_len, snip.data(), snip.size());
    } else if (c == '\\') {
      const char* backslash = "\\";
      append(buf, buf_sz, buf_len, backslash, 1);
    } else {
      return false;
    }
  }
  return true;
}

static int free_and_return_error(char* buf) {
  qio_free((void*)buf);
  return -1;
}

// these should match re2.cc
static const int kMaxArgs = 16;
static const int kVecSize = 1+kMaxArgs;

// this function is loosly based upon RE2::GlobalReplace
static int replace(const RE2& re, const StringPiece& rewrite,
                   const StringPiece& str,
                   int maxreplace,
                   const char** str_out, int64_t* len_out) {
  StringPiece vec[kVecSize];
  int nvec = 1 + RE2::MaxSubmatch(rewrite);
  if (nvec > 1 + re.NumberOfCapturingGroups())
    return -1;
  if (nvec > static_cast<int>(sizeof(vec)/sizeof(StringPiece)))
    return -1;

  const char* p = str.data();
  const char* ep = p + str.size();
  const char* lastend = NULL;

  char* buf = NULL;
  int64_t buf_sz = 0;
  int64_t buf_len = 0;

  int count = 0;
  while (p <= ep) {
    // don't try to replace anything if we are replacing 0
    if (maxreplace == 0)
      break;

    if (!re.Match(str, static_cast<size_t>(p - str.data()),
                  str.size(), RE2::UNANCHORED, vec, nvec))
      break;

    // append the data before the match
    if (p < vec[0].data())
      if (!append(buf, buf_sz, buf_len, p, vec[0].data() - p))
        return free_and_return_error(buf);

    if (vec[0].data() == lastend && vec[0].empty() && count > 0) {
      // quit if we are at the end (trailing \0 added later)
      if (p == ep) {
        break;
      }

      // Disallow empty match at end of last match: skip ahead.
      //
      if (re.options().encoding() == RE2::Options::EncodingUTF8) {
        // append the character
        int nbytes = 0;
        int32_t chr = 0;
        int failed =
          chpl_enc_decode_char_buf_utf8(&chr, &nbytes, p, ep - p, false);
        if (failed == 0) {
          if (!append(buf, buf_sz, buf_len, p, nbytes))
            return free_and_return_error(buf);
          p += nbytes;
          continue;
        }
      }
      // append a character; either not in UTF-8 mode or there
      // is some UTF-8 encoding issue
      if (p < ep)
        if (!append(buf, buf_sz, buf_len, p, 1))
          return free_and_return_error(buf);

      p++;
      continue;
    }

    // append the rewrite
    if (!append_rewrite(buf, buf_sz, buf_len, rewrite, vec, nvec))
      return free_and_return_error(buf);

    // continue with just after the match
    p = vec[0].data() + vec[0].size();
    lastend = p;
    count++;

    // stop if we have replaced too many
    if (count == maxreplace)
      break;
  }

  // append anything after the last match
  if (p < ep)
    if (!append(buf, buf_sz, buf_len, p, ep - p))
      return free_and_return_error(buf);

  // append a null byte
  const char* empty = "";
  if (!append(buf, buf_sz, buf_len, empty, 1))
    return free_and_return_error(buf);

  // return buffer to caller
  *str_out = buf;
  *len_out = buf_len-1; // do not count trailing null byte

  return count;
}

int64_t qio_regex_replace(qio_regex_t* regex, const char* repl, int64_t repl_len, const char* str, int64_t str_len, int64_t maxreplace, const char** str_out, int64_t* len_out)
{
  if (RE2* re = (RE2*) regex->regex) {
    StringPiece rewrite(repl, repl_len);
    StringPiece s(str, str_len);
    return replace(*re, rewrite, s,
                   maxreplace,
                   str_out, len_out);
  }
  return 0;
}

int qio_regex_channel_read_byte(qio_channel_s* ch);
void qio_regex_channel_discard(qio_channel_s* ch, int64_t cur, int64_t min);

int qio_regex_channel_read_byte(qio_channel_s* ch)
{
  int ret;
  ret = qio_channel_read_byte(false, ch);
  //printf("READ BYTE %i %c off=%i\n", ret, ret, (int)qio_channel_offset_unlocked(ch));
  return ret;
}

// cur: what the search process believes to be the current offset
// min: what the search process believes to be the minimum offset
//      that we cannot discard
void qio_regex_channel_discard(qio_channel_s* ch, int64_t cur, int64_t min)
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
  //printf("DISCARD CALLED: advance to %i\n", (int) min);
  qio_channel_advance_unlocked(ch, min - buf);
  //printf("DISCARD CALLED: mark\n");
  qio_channel_mark(false, ch);
  // advance to off.
  //printf("DISCARD CALLED: advance to %i\n", (int) off);
  qio_channel_advance_unlocked(ch, off - min);

  // error checking.
  if ( qio_channel_offset_unlocked(ch) != off ) {
    chpl_internal_error("failed to advance to offset");
  }
}

qioerr qio_regex_channel_match(const qio_regex_t* regex, const int threadsafe, struct qio_channel_s* ch, int64_t maxlen, int anchor, qio_bool can_discard, qio_bool keep_unmatched, qio_bool keep_whole_pattern, qio_regex_string_piece_t* captures, int64_t ncaptures)
{
  re_t* re = (re_t*) regex->regex;
  RE2* re2 = &re->re2;
  qioerr err = NULL;
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
  bool atEOF = false;
  int old_gFileStringAllowBufferSearch;
  MAYBE_STACK_SPACE(FilePiece, caps_onstack);

  if (!re)
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "invalid regex");

  if( ncaptures > INT_MAX || ncaptures < 0 )
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "invalid number of captures");

  start_offset = offset = qio_channel_offset_unlocked(ch);
  end_offset = qio_channel_end_offset_unlocked(ch);

  if (err) goto error;

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
  ci.read_byte_fn = (read_byte_fn_t) &qio_regex_channel_read_byte;
  ci.discard_fn = (discard_fn_t) &qio_regex_channel_discard;
  ci.re = re2;
  if( ncaptures <= 0 ) ci.nmatch = 0;
  else if( ncaptures == 1 ) ci.nmatch = 1;
  else if( ncaptures < INT_MAX ) ci.nmatch = (int) ncaptures;
  else ci.nmatch = INT_MAX;
  ci.can_discard = can_discard;
  ci.keep_unmatched = keep_unmatched;
  ci.keep_whole_pattern = keep_whole_pattern;

  if( anchor == QIO_REGEX_ANCHOR_UNANCHORED ) ranchor = RE2::UNANCHORED;
  else if( anchor == QIO_REGEX_ANCHOR_START ) ranchor = RE2::ANCHOR_START;
  else if( anchor == QIO_REGEX_ANCHOR_BOTH ) ranchor = RE2::ANCHOR_BOTH;

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

  // check for EOF
  err = qio_channel_require_read(false, ch, 1);
  if( qio_err_to_int(err) == EEOF ) {
    atEOF = true;
    err = 0;
  }

  // Require at least 1 byte and at most 1024 bytes.
  need = re2->min_match_length_bytes();
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

  // if we allow MatchFile to search the buffer, given the string "xy", it will
  // incorrectly match "^y". This seems to be an issue with RE2 rather than our
  // shim. As a TODO: we should either fix this (buffer would be faster) or
  // remove the unneeded buffer setup code
  old_gFileStringAllowBufferSearch = gFileStringAllowBufferSearch;
  gFileStringAllowBufferSearch = 0;
  found = re2->MatchFile(text, buffer, ranchor, locs, ncaptures);
  gFileStringAllowBufferSearch = old_gFileStringAllowBufferSearch;

  // Copy capture groups if we found something
  if( found ) {
    for( i = 0; i < ncaptures; i++ ) {
      if( locs[i].start == -1 ) {
        captures[i].offset = -1;
        captures[i].len = 0;
      } else {
        captures[i].offset = locs[i].start;
        captures[i].len = locs[i].size();
      }
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

    // reset the captures if we didn't find anything
    for( i = 0; i < ncaptures; i++ ) {
      captures[i].offset = -1;
      captures[i].len = 0;
    }
  }

markerror:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  // Adjust the error code if nothing was found
  if( err == 0 && ! found ) {
    if (atEOF) {
      err = QIO_EEOF;
    } else {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "no match");
    }
  }

  return err;
}
