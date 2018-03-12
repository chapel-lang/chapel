/*
 * Copyright 2004-2018 Cray Inc.
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

Regular expression support.

The regular expression support is built on top of the RE2 regular expression
library. As such, the exact regular expression syntax available is the syntax
from RE2, which is available within the RE2 project at
https://github.com/google/re2 and included here for your convenience.


Enabling Regular Expression Support
-----------------------------------

Setting the environment variable CHPL_REGEXP to re2 will enable regular
expression support with the RE2 library:

.. code-block:: sh

   export CHPL_REGEXP=re2

Then, rebuild Chapel. The RE2 library will be expanded from a release included
in the Chapel distribution.

.. note::

  if re2 support is not enabled (which is the case in quickstart configurations
  as in :ref:`chapelhome-quickstart`), the functionality described below will
  result in either a compile-time or a run-time error.


Using Regular Expression Support
--------------------------------

.. code-block:: chapel

   use Regexp;
   var myRegexp = compile("a+");

Now you can use these methods on regular expressions: :proc:`regexp.search`,
:proc:`regexp.match`, :proc:`regexp.split`, :proc:`regexp.matches`.

You can also use the string versions of these methods: :proc:`string.search`,
:proc:`string.match`, :proc:`string.split`, or :proc:`string.matches`.

Lastly, you can include regular expressions in the format string for
:proc:`~IO.readf` for searching on QIO channels using the ``%/<regexp>/``
syntax.

Regular Expression Examples
---------------------------

``a+``
 Match one or more ``a`` characters

``[[:space:]]*`` or ``\s*`` (which would be ``"\\s*"`` in a string)
 Match zero or more spaces

``[[:digit:]]+`` or ``\d+`` (which would be ``"\\d+"`` in a string)
 Match one or more digits

``([a-zA-Z0-9]+[[:space:]]+=[[:space:]]+[0-9]+``
 Match sequences of the form *<letters-and-digits> <spaces>* ``=`` *<digits>*


.. _regular-expression-syntax:

RE2 regular expression syntax reference
---------------------------------------

::

  Single characters:
  .            any character, possibly including newline (s=true)
  [xyz]        character class
  [^xyz]       negated character class
  \d           Perl character class (see below)
  \D           negated Perl character class (see below)
  [:alpha:]    ASCII character class
  [:^alpha:]   negated ASCII character class
  \pN          Unicode character class (one-letter name)
  \p{Greek}    Unicode character class
  \PN          negated Unicode character class (one-letter name)
  \P{Greek}    negated Unicode character class

  Composites:
  xy           «x» followed by «y»
  x|y          «x» or «y» (prefer «x»)

  Repetitions:
  x*           zero or more «x», prefer more
  x+           one or more «x», prefer more
  x?           zero or one «x», prefer one
  x{n,m}       «n» or «n»+1 or ... or «m» «x», prefer more
  x{n,}        «n» or more «x», prefer more
  x{n}         exactly «n» «x»
  x*?          zero or more «x», prefer fewer
  x+?          one or more «x», prefer fewer
  x??          zero or one «x», prefer zero
  x{n,m}?      «n» or «n»+1 or ... or «m» «x», prefer fewer
  x{n,}?       «n» or more «x», prefer fewer
  x{n}?        exactly «n» «x»

  Grouping:
  (re)         numbered capturing group
  (?P<name>re) named & numbered capturing group
  (?:re)       non-capturing group
  (?flags)     set flags within current group; non-capturing
  (?flags:re)  set flags during re; non-capturing

  Flags:
  i            case-insensitive (default false)
  m            multi-line mode: «^» and «$» match begin/end line in addition to
                 begin/end text (default false)
  s            let «.» match «\n» (default false)
  U            ungreedy: swap meaning of «x*» and «x*?», «x+» and «x+?», etc.
                 (default false)

  Flag syntax is:
    «xyz»   (set)
    «-xyz»  (clear)
    «xy-z»  (set «xy», clear «z»)

  Empty strings:
  ^            at beginning of text or line («m»=true)
  $            at end of text (like «\z» not «\Z») or line («m»=true)
  \A           at beginning of text
  \b           at word boundary («\w» on one side and «\W», «\A», or «\z» on the
                 other)
  \B           not a word boundary
  \z           at end of text

  Escape sequences:
  \a           bell (== \007)
  \f           form feed (== \014)
  \t           horizontal tab (== \011)
  \n           newline (== \012)
  \r           carriage return (== \015)
  \v           vertical tab character (== \013)
  \*           literal «*», for any punctuation character «*»
  \123         octal character code (up to three digits)
  \x7F         hex character code (exactly two digits)
  \x{10FFFF}   hex character code
  \C           match a single byte even in UTF-8 mode
  \Q...\E      literal text «...» even if «...» has punctuation

  Character class elements:
  x            single character
  A-Z          character range (inclusive)
  \d           Perl character class (see below)
  [:foo:]      ASCII character class «foo»
  \p{Foo}      Unicode character class «Foo»
  \pF          Unicode character class «F» (one-letter name)

  Named character classes as character class elements:
  [\d]         digits (== \d)
  [^\d]        not digits (== \D)
  [\D]         not digits (== \D)
  [^\D]        not not digits (== \d)
  [[:name:]]   named ASCII class inside character class (== [:name:])
  [^[:name:]]  named ASCII class inside negated character class (== [:^name:])
  [\p{Name}]   named Unicode property inside character class (== \p{Name})
  [^\p{Name}]  named Unicode property inside negated character class (==\P{Name})

  Perl character classes:
  \d           digits (== [0-9])
  \D           not digits (== [^0-9])
  \s           whitespace (== [\t\n\f\r ])
  \S           not whitespace (== [^\t\n\f\r ])
  \w           word characters (== [0-9A-Za-z_])
  \W           not word characters (== [^0-9A-Za-z_])

  ASCII character classes::
    Note -- you must use these within a [] group! so if you want
            to match any number of spaces, use [[:space:]]* or \s*

  [:alnum:]    alphanumeric (== [0-9A-Za-z])
  [:alpha:]    alphabetic (== [A-Za-z])
  [:ascii:]    ASCII (== [\x00-\x7F])
  [:blank:]    blank (== [\t ])
  [:cntrl:]    control (== [\x00-\x1F\x7F])
  [:digit:]    digits (== [0-9])
  [:graph:]    graphical (== [!-~] ==
                 [A-Za-z0-9!"#$%&'()*+,\-./:;<=>?@[\\\]^_`{|}~])
  [:lower:]    lower case (== [a-z])
  [:print:]    printable (== [ -~] == [[:graph:]])
  [:punct:]    punctuation (== [!-/:-@[-`{-~])
  [:space:]    whitespace (== [\t\n\v\f\r ])
  [:upper:]    upper case (== [A-Z])
  [:word:]     word characters (== [0-9A-Za-z_])
  [:xdigit:]   hex digit (== [0-9A-Fa-f])

  Unicode character class names--general category:
  C            other
  Cc           control
  Cf           format
  Co           private use
  Cs           surrogate
  L            letter
  Ll           lowercase letter
  Lm           modifier letter
  Lo           other letter
  Lt           titlecase letter
  Lu           uppercase letter
  M            mark
  Mc           spacing mark
  Me           enclosing mark
  Mn           non-spacing mark
  N            number
  Nd           decimal number
  Nl           letter number
  No           other number
  P            punctuation
  Pc           connector punctuation
  Pd           dash punctuation
  Pe           close punctuation
  Pf           final punctuation
  Pi           initial punctuation
  Po           other punctuation
  Ps           open punctuation
  S            symbol
  Sc           currency symbol
  Sk           modifier symbol
  Sm           math symbol
  So           other symbol
  Z            separator
  Zl           line separator
  Zp           paragraph separator
  Zs           space separator

  Unicode character class names--scripts (with explanation where non-trivial):
  Arabic
  Armenian
  Balinese
  Bengali
  Bopomofo
  Braille
  Buginese
  Buhid
  Canadian_Aboriginal
  Carian
  Cham
  Cherokee
  Common       characters not specific to one script
  Coptic
  Cuneiform
  Cypriot
  Cyrillic
  Deseret
  Devanagari
  Ethiopic
  Georgian
  Glagolitic
  Gothic
  Greek
  Gujarati
  Gurmukhi
  Han
  Hangul
  Hanunoo
  Hebrew
  Hiragana
  Inherited    inherit script from previous character
  Kannada
  Katakana
  Kayah_Li
  Kharoshthi
  Khmer
  Lao
  Latin
  Lepcha
  Limbu
  Linear_B
  Lycian
  Lydian
  Malayalam
  Mongolian
  Myanmar
  New_Tai_Lue  aka Simplified Tai Lue
  Nko
  Ogham
  Ol_Chiki
  Old_Italic
  Old_Persian
  Oriya
  Osmanya
  Phags_Pa
  Phoenician
  Rejang
  Runic
  Saurashtra
  Shavian
  Sinhala
  Sundanese
  Syloti_Nagri
  Syriac
  Tagalog
  Tagbanwa
  Tai_Le
  Tamil
  Telugu
  Thaana
  Thai
  Tibetan
  Tifinagh
  Ugaritic
  Vai
  Yi

  Vim character classes:
  \d      digits (== [0-9])
  \D      not «\d»
  \w      word character
  \W      not «\w»

Regular Expression Types and Methods
------------------------------------

 */
module Regexp {

pragma "no doc"
extern type qio_regexp_t;

pragma "no doc"
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

pragma "no doc"
extern proc qio_regexp_null():qio_regexp_t;
private extern proc qio_regexp_init_default_options(ref options:qio_regexp_options_t);
private extern proc qio_regexp_create_compile(str:c_string, strlen:int(64), ref options:qio_regexp_options_t, ref compiled:qio_regexp_t);
private extern proc qio_regexp_create_compile_flags(str:c_string, strlen:int(64), flags:c_string, flagslen:int(64), isUtf8:bool, ref compiled:qio_regexp_t);
pragma "no doc"
extern proc qio_regexp_create_compile_flags_2(str:c_void_ptr, strlen:int(64), flags:c_void_ptr, flagslen:int(64), isUtf8:bool, ref compiled:qio_regexp_t);
private extern proc qio_regexp_retain(const ref compiled:qio_regexp_t);
pragma "no doc"
extern proc qio_regexp_release(ref compiled:qio_regexp_t);
pragma "no doc"

private extern proc qio_regexp_get_options(const ref regexp:qio_regexp_t, ref options: qio_regexp_options_t);
private extern proc qio_regexp_get_pattern(const ref regexp:qio_regexp_t, ref pattern: c_string);
pragma "no doc"
extern proc qio_regexp_get_ncaptures(const ref regexp:qio_regexp_t):int(64);
pragma "no doc"
extern proc qio_regexp_ok(const ref regexp:qio_regexp_t):bool;
private extern proc qio_regexp_error(const ref regexp:qio_regexp_t):c_string;

pragma "no doc"
extern const QIO_REGEXP_ANCHOR_UNANCHORED:c_int;
pragma "no doc"
extern const QIO_REGEXP_ANCHOR_START:c_int;
pragma "no doc"
extern const QIO_REGEXP_ANCHOR_BOTH:c_int;

pragma "no doc"
extern record qio_regexp_string_piece_t {
  var offset:int(64); // counting from 0, -1 means "NULL"
  var len:int(64);
}

private extern proc qio_regexp_string_piece_isnull(ref sp:qio_regexp_string_piece_t):bool;

private extern proc qio_regexp_match(const ref re:qio_regexp_t, text:c_string, textlen:int(64), startpos:int(64), endpos:int(64), anchor:c_int, submatch:_ddata(qio_regexp_string_piece_t), nsubmatch:int(64)):bool;
private extern proc qio_regexp_replace(const ref re:qio_regexp_t, repl:c_string, repllen:int(64), text:c_string, textlen:int(64), startpos:int(64), endpos:int(64), global:bool, ref replaced:c_string, ref replaced_len:int(64)):int(64);

// These two could be folded together if we had a way
// to check if a default argument was supplied
// (or any way to use 'nil' in pass-by-ref)
// This one is documented below.

class BadRegexpError : Error {
  var msg:string;
  proc init(msg: string) {
    this.msg = msg;
  }
  proc message() {
    return msg;
  }
}

pragma "no doc"
proc compile(pattern: string, utf8=true, posix=false, literal=false, nocapture=false, /*i*/ ignorecase=false, /*m*/ multiline=false, /*s*/ dotnl=false, /*U*/ nongreedy=false):regexp throws {

  if CHPL_REGEXP == "none" {
    compilerError("Cannot use Regexp with CHPL_REGEXP=none");
  }

  var err: syserr;
  var ret = compile(pattern, err, utf8, posix, literal, nocapture, ignorecase,
                    multiline, dotnl, nongreedy);

  if err {
    var err_str = qio_regexp_error(ret._regexp);
    var err_msg = err_str:string + " when compiling regexp '" + pattern + "'";
    throw new BadRegexpError(err_msg);
  }
  return ret;
}

/*
   Compile a regular expression. If the optional error argument is provided,
   this routine will return an error code if compilation failed. Otherwise, it
   will halt with an error message.

   :arg pattern: the string regular expression to compile.
                 See :ref:`regular-expression-syntax` for details. Note that
                 you may have to escape backslashes. For example, to
                 get the regular expression ``\s``, you'd have to write
                 ``"\\s"`` because the ``\`` is the escape character within
                 Chapel string literals
   :arg error: (optional) if provided, return an error code instead of halting
               if an error is encountered
   :arg utf8: (optional, default true) set to `true` to create a regular
               expression matching UTF-8; `false` for binary or ASCII only.
   :arg posix: (optional) set to true to disable non-POSIX regular expression
               syntax
   :arg literal: (optional) set to true to treat the regular expression as a
                 literal (ie, create a regexp matching ``pattern`` as a string
                 rather than as a regular expression).
   :arg nocapture: (optional) set to true in order to disable all capture groups
                   in the regular expression
   :arg ignorecase: (optional) set to true in order to ignore case when
                    matching. Note that this can be set inside the regular
                    expression with ``(?i)``.
   :arg multiline: (optional) set to true in order to activate multiline mode
                   (meaning that ``^`` and ``$`` match the beginning and end
                   of a line instead of just the beginning and end of the text.
                   Note that this can be set inside a regular expression
                   with ``(?m)``.
   :arg dotnl: (optional, default false) set to true in order to allow ``.``
               to match a newline. Note that this can be set inside the
               regular expression with ``(?s)``.
   :arg nongreedy: (optional) set to true in order to prefer shorter matches for
                   repetitions; for example, normally x* will match as many x
                   characters as possible and x*? will match as few as possible.
                   This flag swaps the two, so that x* will match as few as
                   possible and x*? will match as many as possible. Note that
                   this flag can be set inside the regular expression with
                   ``(?U)``.

 */
proc compile(pattern: string, out error:syserr, utf8, posix, literal, nocapture, /*i*/ ignorecase, /*m*/ multiline, /*s*/ dotnl, /*U*/ nongreedy):regexp {
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
  qio_regexp_create_compile(pattern.localize().c_str(), pattern.length, opts, ret._regexp);

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
/* mferguson - I agree - I think we should remove this one at some point,
  at least it makes no sense if the string contained here will
  copy the entire larger string, which is what I think we are building.
  */
/*  Regular expression search routines also can work with
    a stringPart, which refers to a substring within another
    string.
 */
pragma "no doc"
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
    in a simple if statement, as in:

    .. code-block:: chapel

      var m:reMatch = ...;
      if m then do_something_if_matched();
      if !m then do_something_if_not_matched();

 */
record reMatch {
  /* true if the regular expression search matched successfully */
  var matched:bool;
  /* 0-based offset into the string or channel that matched; -1 if matched=false */
  var offset:int; // 0-based, -1 if matched==false
  /* the length of the match. 0 if matched==false */
  var length:int; // 0 if matched==false
}

pragma "no doc"
proc _to_reMatch(ref p:qio_regexp_string_piece_t):reMatch {
  if qio_regexp_string_piece_isnull(p) {
    return new reMatch(false, -1, 0);
  } else {
    return new reMatch(true, p.offset, p.len);
  }
}

pragma "no doc"
inline proc !(m: reMatch) return !m.matched;

pragma "no doc"
inline proc _cond_test(m: reMatch) return m.matched;

/*  This function extracts the part of a string matching a regular
    expression or capture group. This method is intended to be
    called on the same string used as the `text` in a regular
    expression search.

    :arg m: a match (e.g. returned by :proc:`regexp.search`)
    :returns: the portion of ``this`` referred to by the match
 */
proc string.this(m:reMatch) {
  if m.matched then return this[m.offset+1..#m.length];
  else return "";
}

/*  This class represents a compiled regular expression. Regular expressions
    are currently cached on a per-thread basis and are reference counted.
    To create a compiled regular expression, use the compile function.

    A regexp can be cast to a string (resulting in the pattern that
    was compiled). A string can be cast to a regexp (resulting in a compiled
    regexp).
  */
pragma "ignore noinit"
record regexp {
  pragma "no doc"
  var home: locale = here;
  pragma "no doc"
  var _regexp:qio_regexp_t = qio_regexp_null();

  /* did this regular expression compile ? */
  proc ok:bool {
    return qio_regexp_ok(_regexp);
  }
  /*

     :returns: a string describing any error encountered when compiling this
               regular expression
   */
  proc error():string {
    return qio_regexp_error(_regexp):string;
  }

  // note - more = overloads are below.
  pragma "no doc"
  proc ref deinit() {
    qio_regexp_release(_regexp);
    _regexp = qio_regexp_null();
  }

  pragma "no doc"
  proc _handle_captures(text: string, matches:_ddata(qio_regexp_string_piece_t), nmatches:int, ref captures) {
    assert(nmatches >= captures.size);
    for param i in 1..captures.size {
      var m = _to_reMatch(matches[i]);
      if captures[i].type == reMatch {
        captures[i] = m;
      } else {
        if m.matched {
          try {
            captures[i] = text[m]:captures[i].type;
          } catch {
            var empty:captures[i].type;
            captures[i] = empty;
          }
        } else {
          var empty:captures[i].type;
          captures[i] = empty;
        }
      }
    }
  }

  // Note - we would only need one version of these routines
  // if we had args ...?k supporting 0 args, or if tuples support zero length

  /*
     Search within the passed text for the first match at any offset to this
     regular expression.  This routine will try matching the regular expression
     at different offsets until a match is found. If you want to only match at
     the beginning of the pattern, you can start your pattern with ``^`` and
     end it with ``$`` or use :proc:`regexp.match`. If a capture group was not
     matched, the corresponding argument will get the default value for its
     type.

     :arg text: a string to search
     :arg captures: (optional) what to capture from the regular expression. These
                    should be strings or types that strings can cast to.
     :returns: an :record:`reMatch` object representing the offset in text
               where a match occurred

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
       got = qio_regexp_match(_regexp, text.from.localize().c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      } else {
       got = qio_regexp_match(_regexp, text.localize().c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      }
      // Now try to coerce the read strings into the captures.
      _handle_captures(text, matches, nmatches, captures);
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches, nmatches);
    }
    return ret;
  }

  // documented in the captures version
  pragma "no doc"
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
       got = qio_regexp_match(_regexp, text.from.localize().c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      } else {
       got = qio_regexp_match(_regexp, text.localize().c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      }
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches, nmatches);
    }
    return ret;
  }

  /*
     Check for a match to this regular expression at the start of the passed
     text. If a capture group was not matched, the corresponding argument will
     get the default value for its type.

     For example, this function can be used to check to see if a string
     fits a particular template:

     .. code-block:: chapel

       if myregexp.match("some string") {
         do_something_if_matched();
       }

     :arg text: a string to search
     :arg captures: what to capture from the regular expression. These should
                    be strings or types that strings can cast to.
     :returns: an :record:`reMatch` object representing the offset in text
               where a match occurred
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
        got = qio_regexp_match(_regexp, text.from.localize().c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      } else {
        got = qio_regexp_match(_regexp, text.localize().c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      }
      // Now try to coerce the read strings into the captures.
      _handle_captures(text, matches, nmatches, captures);
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches, nmatches);
    }
    return ret;
  }

  // documented in the version taking captures.
  pragma "no doc"
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
       got = qio_regexp_match(_regexp, text.from.localize().c_str(), text.from.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      } else {
       got = qio_regexp_match(_regexp, text.localize().c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_START, matches, nmatches);
      }
      // Now return where we matched.
      ret = new reMatch(got, matches[0].offset, matches[0].len);
      _ddata_free(matches, nmatches);
    }
    return ret;
  }


  /*
     Split the text by occurrences of this regular expression.
     If capturing parentheses are used in pattern, then the text of all
     groups in the pattern are also returned as part of the resulting array.
     If *maxsplit* is nonzero, at most maxsplit splits occur, and the
     remaining text is returned as the last element.

     :arg text: a string to split
     :arg maxsplit: if nonzero, the maximum number of splits to do
     :yields: each split portion, one at a time
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
        got = qio_regexp_match(_regexp, text.localize().c_str(), text.length, pos, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
      }

      splits += 1;
      if got && splits <= maxsplits {
        splitstart = matches[0].offset;
        splitend = matches[0].offset + matches[0].len;
      } else {
        splitstart = endpos;
        splitend = endpos;
      }

      if pos < splitstart {
        // Yield splitted value
        yield text[pos+1..splitstart];
      } else {
        yield "";
      }

      if got {
        // Yield capture groups
        for i in 1..ncaptures {
          yield text[new reMatch(
                !qio_regexp_string_piece_isnull(matches[i]),
                matches[i].offset,
                matches[i].len)];
        }
      }

      // Advance to splitend.
      pos = splitend;

      if splits > maxsplits || !got then break;
    }
    on this.home {
      _ddata_free(matches, nmatches);
    }
  }

  /* Enumerates matches in the string as well as capture groups.

     :arg text: the string to search
     :arg captures: (compile-time constant) the size of the captures to return
     :arg maxmatches: the maximum number of matches to return
     :yields: tuples of :record:`reMatch` objects, the 1st is always
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
        got = qio_regexp_match(_regexp, text.localize().c_str(), textLength, cur, endpos, QIO_REGEXP_ANCHOR_UNANCHORED, matches, nmatches);
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
      _ddata_free(matches, nmatches);
    }
  }

  /* Perform the same operation as :proc:`regexp.sub` but return a tuple
     containing the new string and the number of substitutions made.

     :arg repl: replace matches with this string
     :arg text: the text to search and replace within
     :type text: `string`
     :arg global: if true, replace multiple matches
     :returns: a tuple containing (new string, number of substitutions made)
   */
  // TODO -- move subn after sub for documentation clarity
  proc subn(repl:string, text: ?t, global = true ):(string, int)
    where t == string || t == stringPart
  {
    var pos:int;
    var endpos:int;

    if t == stringPart then pos = text.offset;
    else pos = 0;
    endpos = pos + text.length;

    var replaced:c_string;
    var nreplaced:int;
    var replaced_len:int(64);
    if t == stringPart {
      nreplaced = qio_regexp_replace(_regexp, repl.localize().c_str(), repl.length, text.from.localize().c_str(), text.from.length, pos, endpos, global, replaced, replaced_len);
    } else {
      nreplaced = qio_regexp_replace(_regexp, repl.localize().c_str(), repl.length, text.localize().c_str(), text.length, pos, endpos, global, replaced, replaced_len);
    }
    const ret = new string(replaced, needToCopy=false);
    return (ret, nreplaced);
  }

  /*
     Find matches to this regular expression and create a new string in which
     those matches are replaced by repl.

     :arg repl: replace matches with this string
     :arg text: the text to search and replace within
     :type text: `string`
     :arg global: if true, replace multiple matches
     :returns: the new string
   */
  proc sub(repl:string, text: ?t, global = true )
    where t == string || t == stringPart
  {
    var (str, count) = subn(repl, text, global);
    return str;
  }

  pragma "no doc"
  proc writeThis(f) {
    var pattern:string;
    on this.home {
      var patternTemp:c_string;
      qio_regexp_get_pattern(this._regexp, patternTemp);
      pattern = new string(patternTemp);
    }
    // Note -- this is wrong because we didn't quote
    // and there's no way to get the flags
    f <~> "new regexp(\"" <~> pattern <~> "\")";
  }

  pragma "no doc"
  proc readThis(f) {
    var pattern:string;
    // Note -- this is wrong because we didn't quote
    // and there's no way to get the flags
    var litOne = new ioLiteral("new regexp(\"");
    var litTwo = new ioLiteral("\")");
    var err:syserr = ENOERR;
    if(f.read(error=err, litOne, pattern, litTwo)) {
      on this.home {
        var localPattern = pattern.localize();
        var opts:qio_regexp_options_t;
        qio_regexp_init_default_options(opts);
        qio_regexp_create_compile(localPattern.c_str(), localPattern.length, opts, this._regexp);
      }
    }
    if err then
      f.setError(err);
  }
}

pragma "no doc"
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
pragma "no doc"
pragma "init copy fn"
proc chpl__initCopy(x: regexp) {
  on x.home {
    qio_regexp_retain(x._regexp);
  }
  return x;
}

// Cast regexp to string.
pragma "no doc"
inline proc _cast(type t, x: regexp) where t == string {
  var pattern: string;
  on x.home {
    var cs: c_string;
    qio_regexp_get_pattern(x._regexp, cs);
    pattern = new string(cs, needToCopy=false);
  }
  return pattern;
}
// Cast string to regexp
pragma "no doc"
inline proc _cast(type t, x: string) throws where t == regexp {
  return compile(x);
}



/*

   Compile a regular expression and search the receiving string for matches at
   any offset using :proc:`regexp.search`.

   :arg needle: the regular expression to search for
   :arg ignorecase: true to ignore case in the regular expression
   :returns: an :record:`reMatch` object representing the offset in the
             receiving string where a match occurred
 */
proc string.search(needle: string, ignorecase=false):reMatch
{
  // Create a regexp matching the literal for needle
  var re = compile(needle, literal=true, nocapture=true, ignorecase=ignorecase);
  return re.search(this);
}

// documented in the captures version
pragma "no doc"
proc string.search(needle: regexp):reMatch
{
  return needle.search(this);
}

/* Search the receiving string for a regular expression already compiled
   by calling :proc:`regexp.search`. Search for matches at any offset.

   :arg needle: the compiled regular expression to search for
   :arg captures: (optional) what to capture from the regular expression. These
                  should be strings or types that strings can cast to.
   :returns: an :record:`reMatch` object representing the offset in the
             receiving string where a match occurred
 */
proc string.search(needle: regexp, ref captures ...?k):reMatch
{
  return needle.search(this, (...captures));
}

// documented in the captures version
pragma "no doc"
proc string.match(pattern: regexp):reMatch
{
  return pattern.match(this);
}

/* Match the receiving string to a regular expression already compiled
   by calling :proc:`regexp.match`. Only return matches where the match
   encompasses the entire string.

   :arg pattern: the compiled regular expression to match
   :arg captures: (optional) what to capture from the regular expression. These
                  should be strings or types that strings can cast to.
   :returns: an :record:`reMatch` object representing the offset in the
             receiving string where a match occurred
 */

proc string.match(pattern: regexp, ref captures ...?k):reMatch
{
  return pattern.match(this, (...captures));
}

/*
   Split the the receiving string by occurrences of the passed regular
   expression by calling :proc:`regexp.split`.

   :arg pattern: the regular expression to use to split
   :arg maxsplit: if nonzero, the maximum number of splits to do
   :yields: each split portion, one at a time
 */
iter string.split(pattern: regexp, maxsplit: int = 0)
{
  for v in pattern.split(this, maxsplit) {
    yield v;
  }
}

/*
   Enumerates matches in the receiving string as well as capture groups
   by calling :proc:`regexp.matches`.

   :arg pattern: the regular expression to find matches
   :arg captures: (compile-time constant) the size of the captures to return
   :arg maxmatches: the maximum number of matches to return
   :yields: tuples of :record:`reMatch` objects, the 1st is always
            the match for the whole pattern and the rest are the capture groups.

*/
iter string.matches(pattern:regexp, param captures=0, maxmatches:int=max(int))
{
  for v in pattern.matches(this, captures, maxmatches) {
    yield v;
  }
}

} /* end of module */
