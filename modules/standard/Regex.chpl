/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/*

Regular expression support.

The regular expression support is built on top of the RE2 regular expression
library. As such, the exact regular expression syntax available is the syntax
from RE2, which is available within the RE2 project at
https://github.com/google/re2 and included here for your convenience.


Enabling Regular Expression Support
-----------------------------------

Setting the environment variable CHPL_RE2 to bundled will enable regular
expression support with the RE2 library:

.. code-block:: sh

   export CHPL_RE2=bundled

Then, rebuild Chapel. The RE2 library will be expanded from a release included
in the Chapel distribution.

.. note::

  if re2 support is not enabled (which is the case in quickstart configurations
  as in :ref:`chapelhome-quickstart`), the functionality described below will
  result in either a compile-time or a run-time error.


Using Regular Expression Support
--------------------------------

Chapel supports both string and bytes regular expressions.

.. code-block:: chapel

   use Regex;
   var myRegex = compile("a+");   // b"a+" for matching arbitrary bytes values

Now you can use these methods on regular expressions: :proc:`regex.search`,
:proc:`regex.match`, :proc:`regex.split`, :proc:`regex.matches`.

You can also use the string versions of these methods: :proc:`string.search`,
:proc:`string.match`, :proc:`string.split`, or :proc:`string.matches`. Methods
with same prototypes exist for :type:`~Bytes.bytes` type, as well.

Lastly, you can include regular expressions in the format string for
:proc:`~FormattedIO.readf` for searching on QIO channels using the ``%/<regex>/``
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
module Regex {
  private use SysError, CTypes;

pragma "no doc"
extern type qio_regex_t;

pragma "no doc"
extern record qio_regex_options_t {
  var utf8:bool;
  var posix:bool;
  var literal:bool;
  var nocapture:bool;
  // These ones can be set inside the regex
  var ignorecase:bool; // (?i)
  var multiline:bool; // (?m)
  var dotnl:bool; // (?s)
  var nongreedy:bool; // (?U)
}

pragma "no doc"
extern proc qio_regex_null():qio_regex_t;
private extern proc qio_regex_init_default_options(ref options:qio_regex_options_t);
private extern proc qio_regex_create_compile(str:c_string, strlen:int(64), const ref options:qio_regex_options_t, ref compiled:qio_regex_t);
private extern proc qio_regex_create_compile_flags(str:c_string, strlen:int(64), flags:c_string, flagslen:int(64), isUtf8:bool, ref compiled:qio_regex_t);
pragma "no doc"
extern proc qio_regex_create_compile_flags_2(str:c_void_ptr, strlen:int(64), flags:c_void_ptr, flagslen:int(64), isUtf8:bool, ref compiled:qio_regex_t);
private extern proc qio_regex_retain(const ref compiled:qio_regex_t);
pragma "no doc"
extern proc qio_regex_release(ref compiled:qio_regex_t);
pragma "no doc"

private extern proc qio_regex_get_options(const ref regex:qio_regex_t, ref options: qio_regex_options_t);
private extern proc qio_regex_borrow_pattern(const ref regex:qio_regex_t, ref pattern: c_string, ref len_out:int(64));
pragma "no doc"
extern proc qio_regex_get_ncaptures(const ref regex:qio_regex_t):int(64);
pragma "no doc"
extern proc qio_regex_ok(const ref regex:qio_regex_t):bool;
private extern proc qio_regex_error(const ref regex:qio_regex_t):c_string;

pragma "no doc"
extern const QIO_REGEX_ANCHOR_UNANCHORED:c_int;
pragma "no doc"
extern const QIO_REGEX_ANCHOR_START:c_int;
pragma "no doc"
extern const QIO_REGEX_ANCHOR_BOTH:c_int;

pragma "no doc"
extern record qio_regex_string_piece_t {
  var offset:int(64); // counting from 0, -1 means "NULL"
  var len:int(64);
}

private extern proc qio_regex_string_piece_isnull(ref sp:qio_regex_string_piece_t):bool;

private extern proc qio_regex_match(const ref re:qio_regex_t, text:c_string, textlen:int(64), startpos:int(64), endpos:int(64), anchor:c_int, ref submatch:qio_regex_string_piece_t, nsubmatch:int(64)):bool;
private extern proc qio_regex_replace(const ref re:qio_regex_t, repl:c_string, repllen:int(64), text:c_string, textlen:int(64), startpos:int(64), endpos:int(64), global:bool, ref replaced:c_string, ref replaced_len:int(64)):int(64);

// These two could be folded together if we had a way
// to check if a default argument was supplied
// (or any way to use 'nil' in pass-by-ref)
// This one is documented below.

class BadRegexError : Error {
  var msg:string;
  proc init(msg: string) {
    this.msg = msg;
  }
  override proc message() {
    return msg;
  }
}

// Until Issue 17275 is fixed:
deprecated "Regex: 'BadRegexpError' is deprecated; please use 'BadRegexError' instead"
type BadRegexpError = owned BadRegexError;

// When Issue 17275 is fixed:

// pragma "no doc"
// proc BadRegexpError type
// {
//   compilerWarning("Regex: 'BadRegexpError' is deprecated; please use 'BadRegexError' instead");
//   return BadRegexError;
// }

/*
   Compile a regular expression. This routine will throw a
   class:`BadRegexError` if compilation failed.

   :arg pattern: the regular expression to compile. This argument can be string
                 or bytes. See :ref:`regular-expression-syntax` for details.
                 Note that you may have to escape backslashes. For example, to
                 get the regular expression ``\s``, you'd have to write
                 ``"\\s"`` because the ``\`` is the escape character within
                 Chapel string/bytes literals. Note that, Chapel supports
                 triple-quoted raw string/bytes literals, which do not require
                 escaping backslashes. For example ``"""\s"""`` or ``b"""\s"""``
                 can be used.
   :arg posix: (optional) set to true to disable non-POSIX regular expression
               syntax
   :arg literal: (optional) set to true to treat the regular expression as a
                 literal (ie, create a regex matching ``pattern`` as a string
                 rather than as a regular expression).
   :arg noCapture: (optional) set to true in order to disable all capture groups
                   in the regular expression
   :arg ignoreCase: (optional) set to true in order to ignore case when
                    matching. Note that this can be set inside the regular
                    expression with ``(?i)``.
   :arg multiLine: (optional) set to true in order to activate multiline mode
                   (meaning that ``^`` and ``$`` match the beginning and end
                   of a line instead of just the beginning and end of the text.
                   Note that this can be set inside a regular expression
                   with ``(?m)``.
   :arg dotAll: (optional) set to true in order to allow ``.``
               to match a newline. Note that this can be set inside the
               regular expression with ``(?s)``.
   :arg nonGreedy: (optional) set to true in order to prefer shorter matches for
                   repetitions; for example, normally x* will match as many x
                   characters as possible and x*? will match as few as possible.
                   This flag swaps the two, so that x* will match as few as
                   possible and x*? will match as many as possible. Note that
                   this flag can be set inside the regular expression with
                   ``(?U)``.

   :throws BadRegexError: If the argument 'pattern' has syntactical errors.
                          Refer to https://github.com/google/re2/blob/master/re2/re2.h
                          for more details about error codes.
 */
proc compile(pattern: ?t, posix=false, literal=false, noCapture=false,
             /*i*/ ignoreCase=false, /*m*/ multiLine=false, /*s*/ dotAll=false,
             /*U*/ nonGreedy=false): regex(t) throws where t==string || t==bytes {
  use ChplConfig;

  if CHPL_RE2 == "none" {
    compilerError("Cannot use Regex with CHPL_RE2=none");
  }

  var opts:qio_regex_options_t;
  qio_regex_init_default_options(opts);

  // always use UTF8 for strings.
  // For bytes, this is set to false which means use Latin1
  opts.utf8 = t==string;
  opts.posix = posix;
  opts.literal = literal;
  opts.nocapture = noCapture;
  opts.ignorecase = ignoreCase;
  opts.multiline = multiLine;
  opts.dotnl = dotAll;
  opts.nongreedy = nonGreedy;

  var ret: regex(t);
  qio_regex_create_compile(pattern.localize().c_str(), pattern.numBytes, opts, ret._regex);
  if !qio_regex_ok(ret._regex) {
    const patternStr = if t==string then pattern
                                    else pattern.decode(decodePolicy.replace);
    var err_str = qio_regex_error(ret._regex);
    var err_msg: string;
    try! {
      err_msg = createStringWithOwnedBuffer(err_str) +
                  " when compiling regex '" + patternStr + "'";
    }
    throw new owned BadRegexError(err_msg);
  }
  return ret;
}

pragma "no doc"
pragma "last resort"
proc compile(pattern: ?t, posix=false, literal=false, noCapture=false,
             /*i*/ ignoreCase=false, /*m*/ multiLine=false, /*s*/ dotnl=false,
             /*U*/ nonGreedy=false): regex(t) throws where t==string || t==bytes {
  compilerWarning("Regex.compile(): 'dotnl' is deprecated. Please use 'dotAll' instead.");
  return compile(pattern, posix, literal, noCapture, ignoreCase, multiLine, dotnl, nonGreedy);
}

/*  The regexMatch record records a regular expression search match
    or a capture group.

    Regular expression search routines normally return one of these.
    Also, this type can be passed as a capture group argument.
    Lastly, something of type regexMatch can be checked for a match
    in a simple if statement, as in:

    .. code-block:: chapel

      var m:regexMatch = ...;
      if m then do_something_if_matched();
      if !m then do_something_if_not_matched();

    .. warning::
      offset field is deprecated, use byteOffset instead
    .. warning::
      size field is deprecated, use numBytes instead
 */
record regexMatch {
  /* true if the regular expression search matched successfully */
  var matched:bool;
  /* 0-based offset into the string or channel that matched; -1 if matched=false */
  var byteOffset:byteIndex;
  /* the length of the match. 0 if matched==false */
  var numBytes:int;

  pragma "no doc"
  deprecated "field offset is deprecated, use byteOffset instead"
  proc offset:byteIndex {
    return this.byteOffset;
  }

  pragma "no doc"
  deprecated "field size is deprecated, use numBytes instead"
  proc size:int {
    return this.numBytes;
  }
}

pragma "no doc"
proc reMatch type
{
   compilerWarning("Regex: 'reMatch' is deprecated; please use 'regexMatch' instead");
   return regexMatch;
}

pragma "no doc"
proc _to_regexMatch(ref p:qio_regex_string_piece_t):regexMatch {
  if qio_regex_string_piece_isnull(p) {
    return new regexMatch(false, (-1):byteIndex, 0);
  } else {
    return new regexMatch(true, p.offset:byteIndex, p.len);
  }
}

pragma "no doc"
inline operator regexMatch.!(m: regexMatch) return !m.matched;

pragma "no doc"
inline proc regexMatch.chpl_cond_test_method() return this.matched;

/*  This function extracts the part of a string matching a regular
    expression or capture group. This method is intended to be
    called on the same string used as the `text` in a regular
    expression search.

    :arg m: a match (e.g. returned by :proc:`regex.search`)
    :returns: the portion of ``this`` referred to by the match
 */
proc string.this(m:regexMatch) {
  if m.matched then return try! this[m.byteOffset..#m.numBytes];
  else return "";
}

/*  This function extracts the part of a bytes matching a regular
    expression or capture group. This method is intended to be
    called on the same bytes used as the `text` in a regular
    expression search.

    :arg m: a match (e.g. returned by :proc:`regex.search`)
    :returns: the portion of ``this`` referred to by the match
 */
proc bytes.this(m:regexMatch) {
  if m.matched then return this[m.byteOffset:int..#m.numBytes];
  else return b"";
}

private proc serializedType(type exprType) type {
  var expr:exprType;
  return expr.chpl__serialize().type;
}

/* We hold a copy of pattern string/bytes in its serialized form inside
 * this record.
 */
pragma "no doc"
record chpl_serializeHelper {
  type exprType;
  var pattern:serializedType(exprType);
  var options:qio_regex_options_t;
}

 private use IO;

/*  This record represents a compiled regular expression. Regular expressions
    are currently cached on a per-thread basis and are reference counted.
    To create a compiled regular expression, use the proc:`compile` function.

    A string-based regex can be cast to a string (resulting in the pattern that
    was compiled). A string can be cast to a string-based regex (resulting in a
    compiled regex). Same applies for bytes.
  */
pragma "ignore noinit"
record regex {

  pragma "no doc"
  type exprType;
  pragma "no doc"
  var home: locale = here;
  pragma "no doc"
  var _regex:qio_regex_t = qio_regex_null();

  proc init(type exprType) {
    this.exprType = exprType;
  }

  proc init=(x: regex(?)) {
    this.exprType = x.exprType;
    /* always bring the regex local */
    this.home = here;
    /* if it's local, retain and avoid the recompile (thread safe) */
    if (x.home == here) {
      this._regex = x._regex;
      this.complete();
      qio_regex_retain(x._regex);
    } else {
      /* otherwise recompile locally */
      this.complete();
      var serialized = x._serialize();
      this._deserialize(serialized);
    }
  }

  pragma "no doc"
  proc _serialize() {
    var pattern: serializedType(exprType);
    var options: qio_regex_options_t;
    const _regexCopy = _regex;

    on this.home {
      /* NOTE we can't reference `this` inside this on block because
       * it can cause a recursive chpl__serialize loop
       * We also use a borrowed string so that the lifetime of the buffer is as
       * long as the regex itself (and eg. doesn't get freed at the end of this
       * function)
       */
      var patternTemp: c_string;
      var len:int;
      qio_regex_borrow_pattern(_regexCopy, patternTemp, len);
      if exprType == string then {
        try! pattern = createStringWithBorrowedBuffer(patternTemp, len).chpl__serialize();
      }
      else {
        pattern = createBytesWithBorrowedBuffer(patternTemp, len).chpl__serialize();
      }

      var localOptions: qio_regex_options_t;
      qio_regex_get_options(_regexCopy, localOptions);
      options = localOptions;
    }
    return new chpl_serializeHelper(exprType, pattern, options);
  }

  pragma "no doc"
  proc _deserialize(data) {
    const pattern = exprType.chpl__deserialize(data.pattern);
    qio_regex_create_compile(pattern.c_str(),
                             pattern.numBytes,
                             data.options,
                             this._regex);
  }

  pragma "no doc"
  proc chpl__serialize() {
    return _serialize();
  }

  pragma "no doc"
  proc type chpl__deserialize(data) {
    var ret:regex(exprType);
    ret._deserialize(data);
    return ret;
  }

  // note - more = overloads are below.
  pragma "no doc"
  proc ref deinit() {
    qio_regex_release(_regex);
    _regex = qio_regex_null();
  }

  pragma "no doc"
  proc _handle_captures(text: exprType, matches:c_array(qio_regex_string_piece_t, ?nmatches),
                        ref captures) {
    assert(nmatches >= captures.size);
    for param i in 0..captures.size-1 {
      var m = _to_regexMatch(matches[i+1]);
      if captures[i].type == regexMatch {
        captures[i] = m;
      } else {
        if m.matched {
          if captures[i].type == exprType {
            captures[i] = text[m];
          } else {
            try {
              captures[i] = text[m]:captures[i].type;
            } catch {
              var empty:captures[i].type;
              captures[i] = empty;
            }
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
     end it with ``$`` or use :proc:`regex.match`. If a capture group was not
     matched, the corresponding argument will get the default value for its
     type.

     :arg text: a string or bytes to search
     :arg captures: (optional) what to capture from the regular expression.
                    If the class:`regex` was based on string, then, these
                    should be strings or types that strings can cast to. Same
                    applies for bytes.
     :returns: an :record:`regexMatch` object representing the offset in text
               where a match occurred

    */
  proc search(text: exprType, ref captures ...?k):regexMatch
  {
    return _search_match(text, QIO_REGEX_ANCHOR_UNANCHORED, true, captures);
  }

  // documented in the captures version
  pragma "no doc"
  proc search(text: exprType):regexMatch
  {
    var dummy: int;
    return _search_match(text, QIO_REGEX_ANCHOR_UNANCHORED, false, dummy);
  }

  /*
     Check for a match to this regular expression at the start of the passed
     text. If a capture group was not matched, the corresponding argument will
     get the default value for its type.

     For example, this function can be used to check to see if a string
     fits a particular template:

     .. code-block:: chapel

       if myRegex.match("some string") {
         doSomethingIfMatched();
       }

     :arg text: a string or bytes to search
     :arg captures: what to capture from the regular expression.
                    If the class:`regex` was based on string, then, these
                    should be strings or types that strings can cast to. Same
                    applies for bytes.
     :returns: an :record:`regexMatch` object representing the offset in text
               where a match occurred
   */
  proc match(text: exprType, ref captures ...?k):regexMatch
  {
    return _search_match(text, QIO_REGEX_ANCHOR_START, true, captures);
  }

  // documented in the version taking captures.
  pragma "no doc"
  proc match(text: exprType):regexMatch
  {
    var dummy: int;
    return _search_match(text, QIO_REGEX_ANCHOR_START, false, dummy);
  }

  /*
     Check for a match to this regular expression in the full passed text.
     If a capture group was not matched, the corresponding argument will
     get the default value for its type.

     :arg text: a string or bytes to search
     :arg captures: what to capture from the regular expression.
                    If the class:`regex` was based on string, then, these
                    should be strings or types that strings can cast to. Same
                    applies for bytes.
     :returns: an :record:`regexMatch` object representing the offset in text
               where a match occurred
   */
  proc fullMatch(text: exprType, ref captures ...?k):regexMatch
  {
    return _search_match(text, QIO_REGEX_ANCHOR_BOTH, true, captures);
  }

  // documented in the version taking captures.
  pragma "no doc"
  proc fullMatch(text: exprType):regexMatch
  {
    var dummy: int;
    return _search_match(text, QIO_REGEX_ANCHOR_BOTH, false, dummy);
  }

  // Note - we would not need to use has_captures
  // if we had args ...?k supporting 0 args, or if tuples support zero length

  pragma "no doc"
  proc _search_match(text: exprType, anchor: c_int, param has_captures, ref captures):regexMatch
  {
    /* This pattern is a bit ugly, but we'd like to avoid
    unnecessarily copying `this` when local. And we need to maintain
    the lifetime of the copied regex when remote. */
    var regexCopy:regex(exprType);
    if home != here then regexCopy = this;
    const localRegex = if home != here then regexCopy._regex else _regex;
    var ret:regexMatch;
    var pos:byteIndex;
    var endpos:byteIndex;

    pos = 0;
    endpos = pos + text.numBytes;

    param nmatches = if has_captures then 1 + captures.size else 1;
    var matches: c_array(qio_regex_string_piece_t, nmatches);
    var got:bool;
    got = qio_regex_match(localRegex, text.localize().c_str(), text.numBytes,
                          pos:int, endpos:int, anchor,
                          matches[0], nmatches);
    // Now try to coerce the read strings into the captures.
    if has_captures then _handle_captures(text, matches, captures);
    // Now return where we matched.
    ret = new regexMatch(got, matches[0].offset:byteIndex, matches[0].len);
    return ret;
  }

  /*
     Split the text by occurrences of this regular expression.
     If capturing parentheses are used in pattern, then the text of all
     groups in the pattern are also returned as part of the resulting array.
     If *maxsplit* is nonzero, at most maxsplit splits occur, and the
     remaining text is returned as the last element.

     :arg text: a string or bytes to split
     :arg maxsplit: if nonzero, the maximum number of splits to do
     :yields: each split portion, one at a time
   */
  iter split(text: exprType, maxsplit: int = 0)
  {
    var regexCopy:regex(exprType);
    if home != here then regexCopy = this;
    const localRegex = if home != here then regexCopy._regex else _regex;
    var ncaptures = qio_regex_get_ncaptures(localRegex);
    var nmatches = 1 + ncaptures;
    var pos:byteIndex;
    var endpos:byteIndex;
    var last:byteIndex;
    var localText = text.localize();

    var matches = c_malloc(qio_regex_string_piece_t, nmatches);
    defer c_free(matches);

    pos = 0;
    endpos = pos + localText.numBytes;
    last = 0;

    var splits = 0;
    var maxsplits = maxsplit;
    if maxsplit == 0 then maxsplits = max(int);

    while splits < maxsplits && pos <= endpos {
      var got = qio_regex_match(localRegex, localText.c_str(), localText.numBytes,
                                pos:int, endpos:int, QIO_REGEX_ANCHOR_UNANCHORED,
                                matches[0], nmatches);

      if !got then break;

      splits += 1;

      var splitstart:byteIndex = matches[0].offset;
      try! {
        yield text[last..<splitstart];
      }
      last = splitstart + matches[0].len;

      // Yield capture groups
      for i in 1..ncaptures {
          yield text[new regexMatch(
              !qio_regex_string_piece_isnull(matches[i]),
              matches[i].offset:byteIndex,
              matches[i].len)];
      }

      pos = matches[0].offset + max(1, matches[0].len);
    }

    if last <= endpos {
      if last >= text.numBytes {
        yield "":exprType;
      } else {
        try! {
          yield text[last..<endpos];
        }
      }
    }
  }

  /* Enumerates matches in the text as well as capture groups.

     :arg text: the string or bytes to search
     :arg captures: (compile-time constant) the size of the captures to return
     :arg maxmatches: the maximum number of matches to return
     :yields: tuples of :record:`regexMatch` objects, the 1st is always
              the match for the whole pattern and the rest are the capture groups.
   */
  iter matches(text: exprType, param numCaptures=0, maxMatches: int = max(int))
  {
    var regexCopy:regex(exprType);
    if home != here then regexCopy = this;
    const localRegex = if home != here then regexCopy._regex else _regex;
    param nMatches = 1 + numCaptures;
    var matches: c_array(qio_regex_string_piece_t, nMatches);
    var pos:byteIndex;
    var endPos:byteIndex;
    var textLength:int;
    var localText = text.localize();

    pos = 0;
    textLength = localText.numBytes;
    endPos = pos + textLength;

    var nFound = 0;
    var cur = pos;
    while nFound < maxMatches && cur <= endPos {
      var got = qio_regex_match(localRegex, localText.c_str(), textLength,
                                cur:int, endPos:int, QIO_REGEX_ANCHOR_UNANCHORED,
                                matches[0], nMatches);
      if !got then break;
      var ret:nMatches*regexMatch;
      for i in 0..numCaptures {
        ret[i] = new regexMatch(got, matches[i].offset:byteIndex, matches[i].len);
      }
      yield ret;
      cur = matches[0].offset + max(1, matches[0].len);
      nFound += 1;
    }
  }

  pragma "last resort"
  deprecated "regex.matches arguments 'captures' and 'maxmatches' are deprecated. Use 'numCaptures' and/or 'maxMatches instead."
  iter matches(text: exprType, param captures=0, maxmatches: int = max(int))
  {
    for m in matches(text, numCaptures=captures, maxMatches=maxmatches) {
      yield m;
    }
  }

  /* Perform the same operation as :proc:`regex.sub` but return a tuple
     containing the new text and the number of substitutions made.

     :arg repl: replace matches with this string or bytes
     :arg text: the text to search and replace within
     :type text: `string` or `bytes`
     :arg global: if true, replace multiple matches
     :returns: a tuple containing (new text, number of substitutions made)
   */
  proc subn(repl: exprType, text: exprType, global = true ):(exprType, int)
  {
    var regexCopy:regex(exprType);
    if home != here then regexCopy = this;
    const localRegex = if home != here then regexCopy._regex else _regex;
    // TODO -- move subn after sub for documentation clarity
    var pos:byteIndex;
    var endpos:byteIndex;

    pos = 0;
    endpos = pos + text.numBytes;

    var ret: exprType;
    var nreplaced:int;

    var replaced:c_string;
    var replaced_len:int(64);
    nreplaced = qio_regex_replace(localRegex, repl.localize().c_str(),
                                  repl.numBytes, text.localize().c_str(),
                                  text.numBytes, pos:int, endpos:int, global,
                                  replaced, replaced_len);
    if exprType==string {
      try! {
        ret = createStringWithOwnedBuffer(replaced, replaced_len);
      }
    }
    else {
      ret = createBytesWithOwnedBuffer(replaced, replaced_len);
    }

    return (ret, nreplaced);
  }

  /*
     Find matches to this regular expression and create a new string or bytes in
     which those matches are replaced by repl.

     :arg repl: replace matches with this string or bytes
     :arg text: the text to search and replace within
     :type text: `string` or `bytes`
     :arg global: if true, replace multiple matches
     :returns: the new string or bytes
   */
  proc sub(repl: exprType, text: exprType, global = true )
  {
    var (str, count) = subn(repl, text, global);
    return str;
  }

  // TODO this could use _serialize to get the pattern and options
  pragma "no doc"
  proc writeThis(f) throws {
    var pattern:exprType;
    on this.home {
      var patternTemp:c_string;
      var len:int;
      qio_regex_borrow_pattern(this._regex, patternTemp, len);
      if exprType == string then {
        try! {
          pattern = createStringWithNewBuffer(patternTemp, len);
        }
      }
      else {
        pattern = createBytesWithNewBuffer(patternTemp, len);
      }
    }
    // Note -- this is wrong because we didn't quote
    // and there's no way to get the flags
    f <~> "new regex(\"" <~> pattern <~> "\")";
  }

  pragma "no doc"
  proc readThis(f) throws {
    var pattern:exprType;
    // Note -- this is wrong because we didn't quote
    // and there's no way to get the flags
    var litOne = new ioLiteral("new regex(\"");
    var litTwo = new ioLiteral("\")");

    if (f.read(litOne, pattern, litTwo)) then
      on this.home {
        var localPattern = pattern.localize();
        var opts: qio_regex_options_t;

        qio_regex_init_default_options(opts);
        qio_regex_create_compile(localPattern.c_str(),
                                  localPattern.numBytes,
                                  opts,
                                  this._regex);
      }
  }
}

pragma "no doc"
operator regex.=(ref ret:regex(?t), x:regex(t))
{
  // retain -- release
  if x.home == ret.home {
    on x.home {
      qio_regex_retain(x._regex);
      qio_regex_release(ret._regex);
    }
    ret._regex = x._regex;
  } else {
    on ret.home {
      qio_regex_release(ret._regex);
      var serialized = x._serialize();
      ret._deserialize(serialized);
    }
  }
}

// Cast regex to string.
pragma "no doc"
inline operator :(x: regex(string), type t: string) {
  var pattern: t;
  on x.home {
    var cs: c_string;
    var len:int;
    qio_regex_borrow_pattern(x._regex, cs, len);
    if t == string {
      try! {
        pattern = createStringWithNewBuffer(cs, len);
      }
    }
  }
  return pattern;
}

// Cast regex to bytes.
pragma "no doc"
inline operator :(x: regex(bytes), type t: bytes) {
  var pattern: t;
  on x.home {
    var cs: c_string;
    var len:int;
    qio_regex_borrow_pattern(x._regex, cs, len);
    pattern = createBytesWithNewBuffer(cs, len);
  }
  return pattern;
}


// Cast string to regex
pragma "no doc"
inline operator :(x: string, type t: regex(string)) throws {
  return compile(x);
}

// Cast bytes to regex
pragma "no doc"
inline operator :(x: bytes, type t: regex(bytes)) throws {
  return compile(x);
}

/* Search the receiving string for the result of a compiled regular
   expression. Search for matches at any offset.

   :arg pattern: the compiled regular expression to search for
   :returns: a byteIndex representing the offset in the receiving string
             where a match occurred
 */
proc string.find(pattern: regex(string)):byteIndex
{
  return (pattern.search(this)).byteOffset;
}

pragma "no doc"
pragma "last resort"
proc string.search(needle: regex(string)):regexMatch
{
  return needle.search(this);
}

pragma "no doc"
deprecated "string.search is deprecated, use regex search instead"
proc string.search(pattern: regex(string)):regexMatch
{
  return pattern.search(this);
}

/* Search the receiving bytes for the result of a compiled regular
   expression. Search for matches at any offset.

   :arg pattern: the compiled regular expression to search for
   :returns: a byteIndex representing the offset in the receiving bytes
             where a match occurred
 */
proc bytes.find(pattern: regex(bytes)):byteIndex
{
  return (pattern.search(this)).byteOffset;
}

pragma "no doc"
pragma "last resort"
proc bytes.search(needle: regex(bytes)):regexMatch
{
  return needle.search(this);
}

pragma "no doc"
deprecated "bytes.search is deprecated, use regex search instead"
proc bytes.search(pattern: regex(bytes)):regexMatch
{
  return pattern.search(this);
}


pragma "last resort"
deprecated "string.search with needle argument is deprecated, use regex search instead"
proc string.search(needle: regex(string), ref captures ...?k):regexMatch
{
  return needle.search(this, (...captures));
}

/* Search the receiving string for a regular expression already compiled
   by calling :proc:`regex.search`. Search for matches at any offset.

   .. warning:: the search function with a receiving string is deprecated,
                use regex search instead

   :arg pattern: the compiled regular expression to search for
   :arg captures: (optional) what to capture from the regular expression. These
                  should be strings or types that strings can cast to.
   :returns: an :record:`regexMatch` object representing the offset in the
             receiving string where a match occurred
 */

deprecated "string.search is deprecated, use regex search instead"
proc string.search(pattern: regex(string), ref captures ...?k):regexMatch
{
  return pattern.search(this, (...captures));
}


pragma "last resort"
deprecated "bytes.search with needle argument is deprecated, use regex search instead"
proc bytes.search(needle: regex(bytes), ref captures ...?k):regexMatch
{
  return needle.search(this, (...captures));
}

/* Search the receiving bytes for a regular expression already compiled
   by calling :proc:`regex.search`. Search for matches at any offset.

   .. warning:: the search function with receiving bytes is deprecated,
                use regex search instead

   :arg pattern: the compiled regular expression to search for
   :arg captures: (optional) what to capture from the regular expression. These
                  should be bytes or types that bytes can cast to.
   :returns: an :record:`regexMatch` object representing the offset in the
             receiving bytes where a match occurred
 */
deprecated "bytes.search is deprecated, use regex search instead"
proc bytes.search(pattern: regex(bytes), ref captures ...?k):regexMatch
{
  return pattern.search(this, (...captures));
}

// documented in the captures version
pragma "no doc"
deprecated "string.match is deprecated"
proc string.match(pattern: regex(string)):regexMatch
{
  return pattern.match(this);
}

// documented in the captures version
pragma "no doc"
deprecated "bytes.match is deprecated"
proc bytes.match(pattern: regex(bytes)):regexMatch
{
  return pattern.match(this);
}

/* Returns true if the start of the string matches the pattern.

   :arg pattern: the compiled regular expression to match
   :returns: true if string starts with `pattern`, false otherwise
 */
proc string.startsWith(pattern: regex(string)):bool
{
  var rm = pattern.match(this);
  return rm.byteOffset == 0;
}


/* Returns true if the start of the bytes matches the pattern.

   :arg pattern: the compiled regular expression to match
   :returns: true if string starts with `pattern`, false otherwise
 */
proc bytes.startsWith(pattern: regex(bytes)):bool
{
  var rm = pattern.match(this);
  return rm.byteOffset == 0;
}


deprecated "string.match with captures argument is deprecated"
proc string.match(pattern: regex(string), ref captures ...?k):regexMatch
{
  return pattern.match(this, (...captures));
}

deprecated "bytes.match with captures argument is deprecated"
proc bytes.match(pattern: regex(bytes), ref captures ...?k):regexMatch
{
  return pattern.match(this, (...captures));
}

pragma "no doc"
pragma "last resort"
deprecated "the split function with pattern argument is deprecated, use sep instead"
iter string.split(pattern: regex(string), maxsplit: int = 0)
{
   for v in pattern.split(this, maxsplit) {
    yield v;
  }
}


/*
   Split the the receiving string by occurrences of the passed regular
   expression by calling :proc:`regex.split`.

   :arg sep: the regular expression to use to split
   :arg maxsplit: if nonzero, the maximum number of splits to do
   :yields: each split portion, one at a time
 */
iter string.split(sep: regex(string), maxsplit: int = 0)
{
  for v in sep.split(this, maxsplit) {
    yield v;
  }
}

pragma "no doc"
pragma "last resort"
deprecated "the split function with pattern argument is deprecated, use sep instead"
iter bytes.split(pattern: regex(bytes), maxsplit: int = 0)
{
  for v in pattern.split(this, maxsplit) {
    yield v;
  }
}


/*
   Split the the receiving bytes by occurrences of the passed regular
   expression by calling :proc:`regex.split`.

   :arg sep: the regular expression to use to split
   :arg maxsplit: if nonzero, the maximum number of splits to do
   :yields: each split portion, one at a time
 */

iter bytes.split(sep: regex(bytes), maxsplit: int = 0)
{
  for v in sep.split(this, maxsplit) {
    yield v;
  }
}

/*
   Enumerates matches in the receiving string as well as capture groups
   by calling :proc:`regex.matches`.

   .. warning:: the matches function is deprecated

   :arg pattern: the regular expression to find matches
   :arg captures: (compile-time constant) the size of the captures to return
   :arg maxmatches: the maximum number of matches to return
   :yields: tuples of :record:`regexMatch` objects, the 1st is always
            the match for the whole pattern and the rest are the capture groups.

*/
deprecated "the matches function is deprecated"
iter string.matches(pattern:regex(string), param captures=0,
                    maxmatches:int=max(int))
{
  for v in pattern.matches(this, captures, maxmatches) {
    yield v;
  }
}

/*
   Enumerates matches in the receiving bytes as well as capture groups
   by calling :proc:`regex.matches`.

   .. warning:: the matches function is deprecated

   :arg pattern: the regular expression to find matches
   :arg captures: (compile-time constant) the size of the captures to return
   :arg maxmatches: the maximum number of matches to return
   :yields: tuples of :record:`regexMatch` objects, the 1st is always
            the match for the whole pattern and the rest are the capture groups.

*/
deprecated "the matches function is deprecated"
iter bytes.matches(pattern:regex(bytes), param captures=0,
                   maxmatches:int=max(int))
{
  for v in pattern.matches(this, captures, maxmatches) {
    yield v;
  }
}

} /* end of module */
