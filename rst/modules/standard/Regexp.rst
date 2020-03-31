.. default-domain:: chpl

.. module:: Regexp
   :synopsis: Regular expression support.

Regexp
======
**Usage**

.. code-block:: chapel

   use Regexp;



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

Chapel supports both string and bytes regular expressions.

.. code-block:: chapel

   use Regexp;
   var myRegexp = compile("a+");   // b"a+" for matching arbitrary bytes values

Now you can use these methods on regular expressions: :proc:`regexp.search`,
:proc:`regexp.match`, :proc:`regexp.split`, :proc:`regexp.matches`.

You can also use the string versions of these methods: :proc:`string.search`,
:proc:`string.match`, :proc:`string.split`, or :proc:`string.matches`. Methods
with same prototypes exist for :record:`~Bytes.bytes` type, as well.

Lastly, you can include regular expressions in the format string for
:proc:`~FormattedIO.readf` for searching on QIO channels using the ``%/<regexp>/``
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

 

.. class:: BadRegexpError : Error

   .. attribute:: var msg: string

   .. method:: proc init(msg: string)

   .. method:: override proc message()

.. function:: proc compile(pattern: ?t, posix = false, literal = false, noCapture = false, ignoreCase = false, multiLine = false, dotnl = false, nonGreedy = false): regexp(t) throws

   
   Compile a regular expression. This routine will throw a
   class:`BadRegexpError` if compilation failed.
   
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
                 literal (ie, create a regexp matching ``pattern`` as a string
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
   :arg dotnl: (optional, default false) set to true in order to allow ``.``
               to match a newline. Note that this can be set inside the
               regular expression with ``(?s)``.
   :arg nonGreedy: (optional) set to true in order to prefer shorter matches for
                   repetitions; for example, normally x* will match as many x
                   characters as possible and x*? will match as few as possible.
                   This flag swaps the two, so that x* will match as few as
                   possible and x*? will match as many as possible. Note that
                   this flag can be set inside the regular expression with
                   ``(?U)``.
   
   

.. record:: reMatch

   The reMatch record records a regular expression search match
   or a capture group.
   
   Regular expression search routines normally return one of these.
   Also, this type can be passed as a capture group argument.
   Lastly, something of type reMatch can be checked for a match
   in a simple if statement, as in:
   
   .. code-block:: chapel
   
     var m:reMatch = ...;
     if m then do_something_if_matched();
     if !m then do_something_if_not_matched();
   
   


   .. attribute:: var matched: bool

      true if the regular expression search matched successfully 

   .. attribute:: var offset: byteIndex

      0-based offset into the string or channel that matched; -1 if matched=false 

   .. attribute:: var size: int

      the length of the match. 0 if matched==false 

   .. method:: proc length ref

      
      Deprecated - please use :proc:`reMatch.size`.
      

.. method:: proc string.this(m: reMatch)

   This function extracts the part of a string matching a regular
   expression or capture group. This method is intended to be
   called on the same string used as the `text` in a regular
   expression search.
   
   :arg m: a match (e.g. returned by :proc:`regexp.search`)
   :returns: the portion of ``this`` referred to by the match
   

.. method:: proc bytes.this(m: reMatch)

   This function extracts the part of a bytes matching a regular
   expression or capture group. This method is intended to be
   called on the same bytes used as the `text` in a regular
   expression search.
   
   :arg m: a match (e.g. returned by :proc:`regexp.search`)
   :returns: the portion of ``this`` referred to by the match
   

.. record:: regexp

   This class represents a compiled regular expression. Regular expressions
   are currently cached on a per-thread basis and are reference counted.
   To create a compiled regular expression, use the proc:`compile` function.
   
   A string-based regexp can be cast to a string (resulting in the pattern that
   was compiled). A string can be cast to a string-based regexp (resulting in a
   compiled regexp). Same applies for bytes.
   


   .. method:: proc init(type exprType)

   .. method:: proc init=(x: regexp(?))

   .. method:: proc ok: bool

      did this regular expression compile ? 

   .. method:: proc error(): string

      
      
      :returns: a string describing any error encountered when compiling this
                regular expression
      

   .. method:: proc search(text: exprType, ref captures ...?k): reMatch

      
      Search within the passed text for the first match at any offset to this
      regular expression.  This routine will try matching the regular expression
      at different offsets until a match is found. If you want to only match at
      the beginning of the pattern, you can start your pattern with ``^`` and
      end it with ``$`` or use :proc:`regexp.match`. If a capture group was not
      matched, the corresponding argument will get the default value for its
      type.
      
      :arg text: a string or bytes to search
      :arg captures: (optional) what to capture from the regular expression.
                     If the class:`regexp` was based on string, then, these
                     should be strings or types that strings can cast to. Same
                     applies for bytes.
      :returns: an :record:`reMatch` object representing the offset in text
                where a match occurred
      
      

   .. method:: proc match(text: exprType, ref captures ...?k): reMatch

      
      Check for a match to this regular expression at the start of the passed
      text. If a capture group was not matched, the corresponding argument will
      get the default value for its type.
      
      For example, this function can be used to check to see if a string
      fits a particular template:
      
      .. code-block:: chapel
      
        if myRegExp.match("some string") {
          doSomethingIfMatched();
        }
      
      :arg text: a string or bytes to search
      :arg captures: what to capture from the regular expression.
                     If the class:`regexp` was based on string, then, these
                     should be strings or types that strings can cast to. Same
                     applies for bytes.
      :returns: an :record:`reMatch` object representing the offset in text
                where a match occurred
      

   .. itermethod:: iter split(text: exprType, maxsplit: int = 0)

      
      Split the text by occurrences of this regular expression.
      If capturing parentheses are used in pattern, then the text of all
      groups in the pattern are also returned as part of the resulting array.
      If *maxsplit* is nonzero, at most maxsplit splits occur, and the
      remaining text is returned as the last element.
      
      :arg text: a string or bytes to split
      :arg maxsplit: if nonzero, the maximum number of splits to do
      :yields: each split portion, one at a time
      

   .. itermethod:: iter matches(text: exprType, param captures = 0, maxmatches: int = max(int))

      Enumerates matches in the text as well as capture groups.
      
      :arg text: the string or bytes to search
      :arg captures: (compile-time constant) the size of the captures to return
      :arg maxmatches: the maximum number of matches to return
      :yields: tuples of :record:`reMatch` objects, the 1st is always
               the match for the whole pattern and the rest are the capture groups.
      

   .. method:: proc subn(repl: exprType, text: exprType, global = true): (exprType, int)

   .. method:: proc sub(repl: exprType, text: exprType, global = true)

      
      Find matches to this regular expression and create a new string or bytes in
      which those matches are replaced by repl.
      
      :arg repl: replace matches with this string or bytes
      :arg text: the text to search and replace within
      :type text: `string` or `bytes`
      :arg global: if true, replace multiple matches
      :returns: the new string or bytes
      

.. method:: proc string.search(needle: string, ignorecase = false): reMatch

   
   
   Compile a regular expression and search the receiving string for matches at
   any offset using :proc:`regexp.search`.
   
   :arg needle: the regular expression to search for
   :arg ignorecase: true to ignore case in the regular expression
   :returns: an :record:`reMatch` object representing the offset in the
             receiving string where a match occurred
   

.. method:: proc bytes.search(needle: bytes, ignorecase = false): reMatch

   
   
   Compile a regular expression and search the receiving bytes for matches at
   any offset using :proc:`regexp.search`.
   
   :arg needle: the regular expression to search for
   :arg ignorecase: true to ignore case in the regular expression
   :returns: an :record:`reMatch` object representing the offset in the
             receiving bytes where a match occurred
   

.. method:: proc string.search(needle: regexp(string), ref captures ...?k): reMatch

   Search the receiving string for a regular expression already compiled
   by calling :proc:`regexp.search`. Search for matches at any offset.
   
   :arg needle: the compiled regular expression to search for
   :arg captures: (optional) what to capture from the regular expression. These
                  should be strings or types that strings can cast to.
   :returns: an :record:`reMatch` object representing the offset in the
             receiving string where a match occurred
   

.. method:: proc bytes.search(needle: regexp(bytes), ref captures ...?k): reMatch

   Search the receiving bytes for a regular expression already compiled
   by calling :proc:`regexp.search`. Search for matches at any offset.
   
   :arg needle: the compiled regular expression to search for
   :arg captures: (optional) what to capture from the regular expression. These
                  should be bytes or types that bytes can cast to.
   :returns: an :record:`reMatch` object representing the offset in the
             receiving bytes where a match occurred
   

.. method:: proc string.match(pattern: regexp(string), ref captures ...?k): reMatch

   Match the receiving string to a regular expression already compiled by
   calling :proc:`regexp.match`. Note that function only returns a match if
   the start of the string matches the pattern. Use :proc:`string.search`
   to search for the pattern at any offset.
   
   :arg pattern: the compiled regular expression to match
   :arg captures: (optional) what to capture from the regular expression. These
                  should be strings or types that strings can cast to.
   :returns: an :record:`reMatch` object representing the offset in the
             receiving string where a match occurred
   

.. method:: proc bytes.match(pattern: regexp(bytes), ref captures ...?k): reMatch

   Match the receiving bytes to a regular expression already compiled by
   calling :proc:`regexp.match`. Note that function only returns a match if
   the start of the bytes matches the pattern. Use :proc:`bytes.search`
   to search for the pattern at any offset.
   
   :arg pattern: the compiled regular expression to match
   :arg captures: (optional) what to capture from the regular expression. These
                  should be bytes or types that bytes can cast to.
   :returns: an :record:`reMatch` object representing the offset in the
             receiving bytes where a match occurred
   

.. itermethod:: iter string.split(pattern: regexp(string), maxsplit: int = 0)

   
   Split the the receiving string by occurrences of the passed regular
   expression by calling :proc:`regexp.split`.
   
   :arg pattern: the regular expression to use to split
   :arg maxsplit: if nonzero, the maximum number of splits to do
   :yields: each split portion, one at a time
   

.. itermethod:: iter bytes.split(pattern: regexp(bytes), maxsplit: int = 0)

   
   Split the the receiving bytes by occurrences of the passed regular
   expression by calling :proc:`regexp.split`.
   
   :arg pattern: the regular expression to use to split
   :arg maxsplit: if nonzero, the maximum number of splits to do
   :yields: each split portion, one at a time
   

.. itermethod:: iter string.matches(pattern: regexp(string), param captures = 0, maxmatches: int = max(int))

   
   Enumerates matches in the receiving string as well as capture groups
   by calling :proc:`regexp.matches`.
   
   :arg pattern: the regular expression to find matches
   :arg captures: (compile-time constant) the size of the captures to return
   :arg maxmatches: the maximum number of matches to return
   :yields: tuples of :record:`reMatch` objects, the 1st is always
            the match for the whole pattern and the rest are the capture groups.
   

.. itermethod:: iter bytes.matches(pattern: regexp(bytes), param captures = 0, maxmatches: int = max(int))

   
   Enumerates matches in the receiving bytes as well as capture groups
   by calling :proc:`regexp.matches`.
   
   :arg pattern: the regular expression to find matches
   :arg captures: (compile-time constant) the size of the captures to return
   :arg maxmatches: the maximum number of matches to return
   :yields: tuples of :record:`reMatch` objects, the 1st is always
            the match for the whole pattern and the rest are the capture groups.
   

