/*
 * Copyright 2004-2015 Cray Inc.
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

/*
   Regexp Module
 */
module Regexp {

extern type qio_regexp_t;

extern record qio_regexp_options_t {
  var utf8:bool;
  var posix:bool;
  var literal:bool;
  var nocapture:bool;
  // These ones can be set inside the regexp
  var ignorecase:bool; // (?i)
  var multiline:bool; // (?m)
  var dotnl:bool; // (?s)
  var nongreedy:bool; // (?U)
}

extern proc qio_regexp_null():qio_regexp_t;
extern proc qio_regexp_init_default_options(ref options:qio_regexp_options_t);
extern proc qio_regexp_create_compile(str:c_string, strlen:int(64), ref options:qio_regexp_options_t, ref compiled:qio_regexp_t);
extern proc qio_regexp_create_compile_flags(str:c_string, strlen:int(64), flags:c_string, flagslen:int(64), isUtf8:bool, ref compiled:qio_regexp_t);
extern proc qio_regexp_create_compile_flags_2(str:c_void_ptr, strlen:int(64), flags:c_void_ptr, flagslen:int(64), isUtf8:bool, ref compiled:qio_regexp_t);
extern proc qio_regexp_retain(const ref compiled:qio_regexp_t);
extern proc qio_regexp_release(ref compiled:qio_regexp_t);

extern proc qio_regexp_get_options(const ref regexp:qio_regexp_t, ref options: qio_regexp_options_t);
extern proc qio_regexp_get_pattern(const ref regexp:qio_regexp_t, ref pattern: c_string);
extern proc qio_regexp_get_ncaptures(const ref regexp:qio_regexp_t):int(64);
extern proc qio_regexp_ok(const ref regexp:qio_regexp_t):bool;
extern proc qio_regexp_error(const ref regexp:qio_regexp_t):c_string;

extern const QIO_REGEXP_ANCHOR_UNANCHORED:c_int;
extern const QIO_REGEXP_ANCHOR_START:c_int;
extern const QIO_REGEXP_ANCHOR_BOTH:c_int;

extern record qio_regexp_string_piece_t {
  var offset:int(64); // counting from 0, -1 means "NULL"
  var len:int(64);
}

extern proc qio_regexp_string_piece_isnull(ref sp:qio_regexp_string_piece_t):bool;

extern proc qio_regexp_match(ref re:qio_regexp_t, text:c_string, textlen:int(64), startpos:int(64), endpos:int(64), anchor:c_int, submatch:_ddata(qio_regexp_string_piece_t), nsubmatch:int(64)):bool;
extern proc qio_regexp_replace(ref re:qio_regexp_t, repl:c_string, repllen:int(64), text:c_string, textlen:int(64), startpos:int(64), endpos:int(64), global:bool, ref replaced:c_string_copy, ref replaced_len:int(64)):int(64);

// These two could be folded together if we had a way
// to check if a default argument was supplied
// (or any way to use 'nil' in pass-by-ref)
/** Compile a regular expression. This routine will halt if compilation failed
 */
proc compile(pattern: string, utf8=true, posix=false, literal=false, nocapture=false, /*i*/ ignorecase=false, /*m*/ multiline=false, /*s*/ dotnl=false, /*U*/ nongreedy=false):regexp {
  var opts:qio_regexp_options_t;
  qio_regexp_init_default_options(opts);
  opts.utf8 = utf8;
  opts.posix = posix;
  opts.literal = literal;
  opts.nocapture = nocapture;
  opts.ignorecase = ignorecase;
  opts.multiline = multiline;
  opts.dotnl = dotnl;
  opts.nongreedy = nongreedy;

  var ret:regexp;
  qio_regexp_create_compile(pattern.c_str(), pattern.length, opts, ret._regexp);
  if ! qio_regexp_ok(ret._regexp) {
    var err_str = qio_regexp_error(ret._regexp);
    __primitive("chpl_error", "Error " + err_str + " when compiling regexp '" + pattern + "'");
  }
  return ret;
}

/*  Compile a regular expression and retrieve an error code if compilation
    failed.
 */
proc compile(pattern: string, out error:syserr, utf8=true, posix=false, literal=false, nocapture=false, /*i*/ ignorecase=false, /*m*/ multiline=false, /*s*/ dotnl=false, /*U*/ nongreedy=false):regexp {
  var opts:qio_regexp_options_t;
  qio_regexp_init_default_options(opts);
  opts.utf8 = utf8;
  opts.posix = posix;
  opts.literal = literal;
  opts.nocapture = nocapture;
  opts.ignorecase = ignorecase;
  opts.multiline = multiline;
  opts.dotnl = dotnl;
  opts.nongreedy = nongreedy;

  var ret:regexp;
  qio_regexp_create_compile(pattern, pattern.length, opts, ret._regexp);

  if qio_regexp_ok(ret._regexp) {
    error = ENOERR;
  } else {
    error = qio_format_error_bad_regexp();
  }

  return ret;
}

/***
 *** sungeun: AFAIK, there are no tests programs or examples that use
 *** this.  Depending on how it is generally used, it might be
 *** easier/cheaper to use a c_string here and provide a custom
 *** constructor.
 ***/
/*  Regular expression search routines also can work with
    a stringPart, which refers to a substring within another
    string.
 */
record stringPart {
  var offset:int;
  var length:int;
  var from:string;
}

/*  The reMatch record records a regular expression search match
    or a capture group.
    Regular expression search routines normally return one of these.
    Also, this type can be passed as a capture group argument.
    Lastly, something of type reMatch can be checked for a match
    in a simple if statement, as in::

      var m:reMatch = ....;
      if m then do_something_if_matched();
      if !m then do_something_if_not_matched();

 */
record reMatch {
  var matched:bool;
  var offset:int; // 0-based, -1 if matched==false
  var length:int; // 0 if matched==false
}

proc _to_reMatch(ref p:qio_regexp_string_piece_t):reMatch {
  if qio_regexp_string_piece_isnull(p) {
    return new reMatch(false, -1, 0); 
  } else {
    return new reMatch(true, p.offset, p.len);
  }
}

inline proc !(m: reMatch) return !m.matched;
inline proc _cond_test(m: reMatch) return m.matched;

/*  Use someString.substring(anReMatch) to extract the part
    of a string matching a regular expression or capture group.
 */
proc string.substring(m:reMatch) {
  if m.matched then return this.substring(m.offset+1..#m.length);
  else return "";
}

/*  This class represents a compiled regular expression. Regular expressions
    are cached on a per-thread basis and are reference counted.
    To create a compiled regular expression, use the compile function.
  */
pragma "ignore noinit"
record regexp {
  var home: locale = here;
  var _regexp:qio_regexp_t = qio_regexp_null();

  proc ok:bool {
    return qio_regexp_ok(_regexp);
  }
  proc error():string {
    return toString(qio_regexp_error(_regexp));
  }

  // note - more = overloads are below.
  proc ~regexp() {
    qio_regexp_release(_regexp);
    _regexp = qio_regexp_null();
  }

  proc _handle_captures(text: string, matches:_ddata(qio_regexp_string_piece_t), nmatches:int, ref captures) {
    assert(nmatches >= captures.size);
    for param i in 1..captures.size {
      var m = _to_reMatch(matches[i]);
      if captures[i].type == reMatch {
        captures[i] = m;
      } else {
        if m.matched {
          captures[i] = text.substring(m):captures[i].type;
        } else {
          var empty:captures[i].type;
          captures[i] = empty;
        }
      }
    }
  }

  // Note - we would only need one version of these routines
  // if we had args ...?k supporting 0 args, or if tuples support zero length
  /*  Search within the passed text (which must be a string or a stringPart)
      for the first match at any offset to this regular expression. Returns a
      match object. This routine will try matching the regular expression at
      different offsets until a match is found. If you want to only match at
      the beginning of the pattern, you can start your pattern with ^ or
      use regexp.match. The capture group arguments here can be strings
      or any type that can reasonably cast from string. If a capture group
      was not matched, the corresponding argument will get the default value
      for its type.
    */
  proc search(text: ?t, ref captures ...?k):reMatch
    where t == string || t == stringPart
  {
    var ret:reMatch;
    on this.home {
      var pos:int;
      var endpos:int;

      if t == stringPart then pos = text.offset;
      else pos = 0;
      endpos = pos + text.length;

      var matches:_ddata(qio_regexp_string_piece_t);
      var nmatches = 1 + captures.size;
      matches = _ddata_allocate(qio_regexp_string_piece_t, nmatches);
      var got:bool;
      if t == stringPart {
       got = qio_regexp_match(_regexp, text.from.c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      } else {
       got = qio_regexp_match(_regexp, text.c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      }
      // Now try to coerce the read strings into the captures.
      _handle_captures(text, matches, nmatches, captures);
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches);
    }
    return ret;
  }
  /*  Search for the first match at any offset without returning any capture
      groups.
    */
  proc search(text: ?t):reMatch
    where t == string || t == stringPart
  {
    var ret:reMatch;
    on this.home {
      var pos:int;
      var endpos:int;

      if t == stringPart then pos = text.offset;
      else pos = 0;
      endpos = pos + text.length;

      var matches:_ddata(qio_regexp_string_piece_t);
      var nmatches = 1;
      matches = _ddata_allocate(qio_regexp_string_piece_t, nmatches);
      var got:bool;
      if t == stringPart {
       got = qio_regexp_match(_regexp, text.from.c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      } else {
       got = qio_regexp_match(_regexp, text.c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      }
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches);
    }
    return ret;
  }

  /*  Check for a match to this regular expression at the start of the
      passed text. As with regexp.search, the capture group arguments
      here can be strings or any type that can reasonably cast from string. If
      a capture group was not matched, the corresponding argument will get the
      default value for its type.

      You can do::

       if myregexp.match("some string") {
         ... do something if matched;
       }
   */
  proc match(text: ?t, ref captures ...?k):reMatch
    where t == string || t == stringPart
  {
    var ret:reMatch;
    on this.home {
      var pos:int;
      var endpos:int;

      if t == stringPart then pos = text.offset;
      else pos = 0;
      endpos = pos + text.length;

      var matches:_ddata(qio_regexp_string_piece_t);
      var nmatches = 1 + captures.size;
      matches = _ddata_allocate(qio_regexp_string_piece_t, nmatches);
      var got:bool;
      if t == stringPart {
        got = qio_regexp_match(_regexp, text.from.c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      } else {
        got = qio_regexp_match(_regexp, text.c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      }
      // Now try to coerce the read strings into the captures.
      _handle_captures(text, matches, nmatches, captures);
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches);
    }
    return ret;
  }
  proc match(text: ?t):reMatch
    where t == string || t == stringPart
  {
    var ret:reMatch;
    on this.home {
      var pos:int;
      var endpos:int;

      if t == stringPart then pos = text.offset;
      else pos = 0;
      endpos = pos + text.length;

      var matches:_ddata(qio_regexp_string_piece_t);
      var nmatches = 1;
      matches = _ddata_allocate(qio_regexp_string_piece_t, nmatches);
      var got:bool;
      if t == stringPart {
       got = qio_regexp_match(_regexp, text.from.c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      } else {
       got = qio_regexp_match(_regexp, text.c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      }
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches);
    }
    return ret;
  }


  /* Split the text by occurrences of this regular expression.
     If capturing parentheses are used in pattern, then the text of all
     groups in the pattern are also returned as part of the resulting array.
     If maxsplit is nonzero, at most maxsplit splits occur, and the
     remaining text is returned as the last element.
   */
  iter split(text: ?t, maxsplit: int = 0) 
    where t == string || t == stringPart 
  {
    var matches:_ddata(qio_regexp_string_piece_t);
    var ncaptures = qio_regexp_get_ncaptures(_regexp);
    var nmatches = 1 + ncaptures;
    var pos:int;
    var endpos:int;

    on this.home {
      matches = _ddata_allocate(qio_regexp_string_piece_t, nmatches);
    }

    if t == stringPart then pos = text.offset;
    else pos = 0;
    endpos = pos + text.length;

    var splits = 0;
    var maxsplits = maxsplit;
    if maxsplit == 0 then maxsplits = max(int);

    while true {
      var splitstart = 0;
      var splitend = 0;
      var got:bool;
      on this.home {
        // This doesn't have a case for stringPart.  Mistake?
        got = qio_regexp_match(_regexp, text.c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      }

      splits += 1;
      if got && splits <= maxsplits {
        splitstart = matches[0].offset;
        splitend = matches[0].offset + matches[0].len;
      } else {
        splitstart = endpos;
        splitend = endpos;
      }

      if pos < endpos {
        // Yield splitted value
        yield text.substring(pos+1..splitstart);
      } else {
        yield "";
      }

      if got {
        // Yield capture groups
        for i in 1..ncaptures {
          yield text.substring(new reMatch(
                !qio_regexp_string_piece_isnull(matches[i]),
                matches[i].offset,
                matches[i].len));
        }
      }

      // Advance to splitend.
      pos = splitend;

      if splits > maxsplits || !got then break;
    }
    on this.home {
      _ddata_free(matches);
    }
  }
  /* Enumerates matches in the string as well as capture groups.

     Returns tuples of reMatch objects, the 1st is always
     the match for the whole pattern and the rest are the capture groups.
   */
  iter matches(text: ?t, param captures=0, maxmatches: int = max(int)) 
    where t == string || t == stringPart 
  {
    var matches:_ddata(qio_regexp_string_piece_t);
    var nmatches = 1 + captures;
    var pos:int;
    var endpos:int;
    var textLength:int;
    on this.home {
      matches = _ddata_allocate(qio_regexp_string_piece_t, nmatches);
    }

    if t == stringPart then pos = text.offset;
    else pos = 0;
    textLength = text.length;
    endpos = pos + textLength;

    var nfound = 0; 
    var cur = pos;
    while nfound < maxmatches && cur < endpos {
      var got:bool;
      on this.home {
        // This doesn't have a case for stringPart.  Mistake?
        got = qio_regexp_match(_regexp, text.c_str(), textLength, cur, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      }
      if !got then break;
      param nret = captures+1;
      var ret:nret*reMatch;
      for i in 0..captures {
        ret[i+1] = new reMatch(got, matches[i].offset, matches[i].len);
      }
      yield ret;
      cur = matches[0].offset + matches[0].len;
    }
    on this.home {
      _ddata_free(matches);
    }
  }
  proc subn(repl:string, text: ?t, global = true ):(string, int)
    where t == string || t == stringPart 
  {
    var pos:int;
    var endpos:int;

    if t == stringPart then pos = text.offset;
    else pos = 0;
    endpos = pos + text.length;

    var replaced:c_string_copy;
    var nreplaced:int; 
    var replaced_len:int(64); 
    if t == stringPart {
      nreplaced = qio_regexp_replace(_regexp, repl.c_str(), repl.length, text.from.c_str(), text.from.length, pos, endpos, global, replaced, replaced_len);
    } else {
      nreplaced = qio_regexp_replace(_regexp, repl.c_str(), repl.length, text.c_str(), text.length, pos, endpos, global, replaced, replaced_len);
    }
    const ret = toString(replaced);
    return (ret, nreplaced);
  }
  proc sub(repl:string, text: ?t, global = true )
    where t == string || t == stringPart 
  {
    var (str, count) = subn(repl, text, global);
    return str;
  }
  proc writeThis(f: Writer) {
    var pattern:c_string;
    on this.home {
      qio_regexp_get_pattern(this._regexp, pattern);
    }
    // Note -- this is wrong because we didn't quote
    // and there's no way to get the flags
    f.write("new regexp(\"", pattern, "\")");
  }
  proc readThis(f: Reader) {
    var pattern:c_string;
    // Note -- this is wrong because we didn't quote
    // and there's no way to get the flags
    var litOne = new ioLiteral("new regexp(\"");
    var litTwo = new ioLiteral("\")");
    if(f.read(litOne, pattern, litTwo)) {
      on this.home {
        var opts:qio_regexp_options_t;
        qio_regexp_init_default_options(opts);
        qio_regexp_create_compile(pattern, pattern.length, opts, this._regexp);
      }
    }
  }
}

proc =(ref ret:regexp, x:regexp)
{
  // retain -- release
  if x.home == ret.home {
    on x.home {
      qio_regexp_retain(x._regexp);
      qio_regexp_release(ret._regexp);
    }
    ret._regexp = x._regexp;
  } else {
    var pattern:c_string;
    var options:qio_regexp_options_t;

    on ret.home {
      qio_regexp_release(ret._regexp);
    }
    on x.home {
      qio_regexp_get_pattern(x._regexp, pattern);
      qio_regexp_get_options(x._regexp, options);
    }

    qio_regexp_create_compile(pattern, pattern.length, options, ret._regexp);
  }
}

// TODO -- shouldn't have to write this this way!
pragma "init copy fn"
proc chpl__initCopy(x: regexp) {
  on x.home {
    qio_regexp_retain(x._regexp);
  }
  return x;
}

// Cast regexp to string.
inline proc _cast(type t, x: regexp) where t == string {
  var pattern: string;
  on x.home {
    var cs: c_string_copy;
    qio_regexp_get_pattern(x._regexp, cs);
    pattern = toString(cs);
  }
  return pattern;
}
// Cast string to regexp
inline proc _cast(type t, x: string) where t == regexp {
  return compile(x);
}


// string methods for searching
proc string.search(needle: string, ignorecase=false):reMatch
{
  // Create a regexp matching the literal for needle
  var re = compile(needle, literal=true, nocapture=true, ignorecase=ignorecase);
  return re.search(this);
}
proc string.search(needle: regexp):reMatch
{
  return needle.search(this);
}
proc string.search(needle: regexp, ref captures ...?k):reMatch
{
  return needle.search(this, (...captures));
}
proc string.match(pattern: regexp):reMatch
{
  return pattern.match(this);
}
proc string.match(pattern: regexp, ref captures ...?k):reMatch
{
  return pattern.match(this, (...captures));
}

iter string.split(pattern: regexp, maxsplit: int = 0)
{
  for v in pattern.split(this, maxsplit) {
    yield v;
  }
}

iter string.matches(pattern:regexp, param captures=0, maxmatches:int=max(int))
{
  for v in pattern.matches(this, captures, maxmatches) {
    yield v;
  }
}

} /* end of module */
