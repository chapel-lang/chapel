#!/usr/bin/perl -w
#
# sclc -- source code line counter (actually it counts more than just lines)
#
# Created 06/09/95 by Brad Appleton &lt;bradapp@enteract.com&gt;
# Patched for Matlab and ZPL 11/01/04 by Bill Mann &lt;wfmann@alum.mit.edu&gt;
# Patched for FORTRAN 11/03/04 by Bill Mann &lt;wfmann@alum.mit.edu&gt;
# Added approximate token counting  1/20/05  Bill Mann
# Added my impression of Chapel  1/20/05  Bill Mann
#
#-------------------------------------------------------------------------
# COPYING
# =======
# This file/program is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself. Please refer to the
# license that came with your Perl distribution for more details.
#
# DISCLAIMER
# ===========
# This software is distributed in the hope that it will be useful, but
# is provided "AS IS" WITHOUT WARRANTY OF ANY KIND, either expressed or
# implied, INCLUDING, without limitation, the implied warranties of
# MERCHANTABILITY and FITNESS FOR A PARTICULAR PURPOSE.
#
# The ENTIRE RISK as to the quality and performance of the software
# IS WITH YOU (the holder of the software).  Should the software prove
# defective, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
# CORRECTION.
#
# IN NO EVENT WILL ANY COPYRIGHT HOLDER OR ANY OTHER PARTY WHO MAY CREATE,
# MODIFY, OR DISTRIBUTE THE SOFTWARE BE LIABLE OR RESPONSIBLE TO YOU OR TO
# ANY OTHER ENTITY FOR ANY KIND OF DAMAGES (no matter how awful - not even
# if they arise from known or unknown flaws in the software).
##########################################################################

package Sclc;

use strict;
use Config;
use File::Basename qw(&amp;basename &amp;dirname);

## Get basename and dirname of program path
my $Is_WinDos     = ( (uc $Config{'osname'}) =~ /(?:MSDOS|MSWIN|WIN32)/ );
my $Path_Sep      = ( $Is_WinDos ? "\\" : "/" );
my $PathList_Sep  = ( $Is_WinDos ? ";" : ":" );
my ($NAME, $EXECDIR) = (basename($0), dirname($0) || ".");

my @Languages = (
      'Ada',
      'Assembly',
      'Awk',
      'C',
      'C++',
      'Chapel',
      'Eiffel',
      'FORTRAN',
      'Java',
      'Lisp',
      'Matlab',
      'Pascal',
      'Perl',
      'Tcl',
      'ZPL',
      'shell',
      'make',
);

=head1 NAME

sclc -- Source-code line counter

=head1 SYNOPSIS

 sclc [-delim-ignore] [-counts SPEC] [-sections SPEC] [-language [EXT=]LANG]
      [-name REGEXP] [-except REGEXP] [-recurse] [-filelist] [-ignore]
      [-diff] [-fromversion SELECTOR] [-toversion SELECTOR] [-pred]
      [-prefix REGEXP] [-separate REGEXP] [-vcstring STRING] [-output FILE]
      [FILE ...]

=head1 OPTIONS AND ARGUMENTS

=over 6

=item B&lt;-Help&gt;

Print this help message and exit.

=item B&lt;-language&gt; [I&lt;EXT&gt;=]I&lt;LANG&gt;

Assume that any file with a suffix of "I&lt;EXT&gt;" is a C&lt;LANG&gt; source file
(note that you must supply any leading '.'). If the "I&lt;EXT&gt;" begins
with a slash ('/'), then the name that follows it is considered to
be the name of an interpreter that is invoked using "#!I&lt;path&gt;/I&lt;EXT&gt;"
as the first line of the script (and if the language can't be determined
from the extension, then the first line of the file will be used). If
"I&lt;EXT&gt;=" is omitted then C&lt;LANG&gt; will be the default language for any files
whose language cannot be determined.  C&lt;LANG&gt; must be one of the following:

=over 3

=item S&lt;&gt;

Ada,
Assembly,
Awk,
C,
C++,
Chapel,
Eiffel,
FORTRAN,
Java,
Lisp,
Matlab,
Pascal,
Perl,
Tcl,
ZPL,
shell,
make

=back

Language names may I&lt;not&gt; be abbreviated. This option may be specified
multiple times.

=item B&lt;-delim-ignore&gt;

Ignore all statement delimiters. This means that lines containing
nothing but statement delimiters/terminators are *not* considered
lines of code. For C/C++ this would have the effect of ignoring
lines containing nothing but ';', '{', and '}'.

=item B&lt;-diff&gt;

Treat the input as output from B&lt;diff(1)&gt; and report counts for both
inserted and deleted source. Note that the B&lt;-diff&gt; option is implied
by any of B&lt;-pred&gt;, B&lt;-fromversion&gt; or B&lt;-toversion&gt;.

=item B&lt;-ignore&gt;

Don't bother with files whose language can't be determined.

=item B&lt;-filelist&gt;

Assume that the source files to read are listed in the contents of
the files given on the command line (filelist is read from standard
input if no files are given). This is useful if you wish to use the
output of commands Like B&lt;find(1)&gt; as input to this command.

=item B&lt;-name&gt; I&lt;REGEXP&gt;

Only look at files whose filename (not including the directory path)
completely match the given perl-style regular expression. Does not
apply to directories when B&lt;-recurse&gt; is used. May be specified
multiple times (for a description of Perl's regular expression
syntax, invoke C&lt;man perlre&gt; or C&lt;perldoc perlre&gt;).

=item B&lt;-except&gt; I&lt;REGEXP&gt;

Don't look at files whose filename (not including the directory path)
completely match the given perl-style regular expression (even if
it would be matched by a B&lt;-name&gt; expression). Does not apply to
directories when B&lt;-recurse&gt; is used. May be specified multiple
times  (for a description of Perl's regular expression
syntax, invoke C&lt;man perlre&gt; or C&lt;perldoc perlre&gt;).

=item B&lt;-prefix&gt; I&lt;REGEXP&gt;

Ignore/strip the leading portion of each line that matches the given
perl-style regular expression. Do I&lt;not&gt; use a leading '^' unless you
mean to match the caret character itself (for a description of Perl's
regular expression syntax, invoke C&lt;man perlre&gt; or C&lt;perldoc perlre&gt;)

=item B&lt;-recurse&gt;

For every "source" file that is actually a directory, read all
source files in the directory.

=item B&lt;-fromversion&gt; I&lt;SELECTOR&gt;

When used with B&lt;-diff&gt;, assumes that I&lt;SELECTOR&gt; is a valid ClearCase
version selector and uses it in conjunction with B&lt;cdiff&gt; as the
"from" version of each file specified. Only one of C&lt;-fromversion&gt;
and C&lt;-pred&gt; may be specified!

=item B&lt;-toversion&gt; I&lt;SELECTOR&gt;

When used with B&lt;-diff&gt;, assumes that I&lt;SELECTOR&gt; is a valid ClearCase
version selector and uses it in conjunction with B&lt;cdiff&gt; as the
"to" version of each file specified.

=item B&lt;-pred&gt;

When used with B&lt;-diff&gt;, assumes that each file is an element in a
ClearCase VOB and uses it in conjunction with B&lt;cdiff&gt; as the "from"
version of each file specified (note that the predecessor version
is chosen relative to the "to" version of the file). Only one of
B&lt;-pred&gt; and B&lt;-fromversion&gt; may be specified!

=item B&lt;-separate&gt; I&lt;REGEXP&gt;

Assume that whenever a source line is encountered that completely
matches the given perl-style regular expression, it indicates the
end of the current source context. Useful if you are trying to
count lines from the output of a command like B&lt;diff(1)&gt; and you
dont want a comment or quote in one changed section to be treated
as if it continued into the next block of modifications. May be
specified multiple times (for a description of Perl's regular expression
syntax, invoke C&lt;man perlre&gt; or C&lt;perldoc perlre&gt;).

=item B&lt;-counts&gt; I&lt;SPEC&gt;

Specify which counts/columns to display in the output. I&lt;SPEC&gt;
may contain any of the following (separated by '+'):

   "Lines"  : print the total # of lines
   "Blank"  : print the total # of blank-lines
   "Cmnts"  : print the total # of comments
   "NCSL"   : print the total # of non-comment source lines
   "KPtoks" : print the total # of keyword pseudo-tokens
   "SPtoks" : print the total # of symbol pseudo-tokens
   "CPtoks" : print the total # of constant pseudo-tokens
   "GPtoks" : print the total # of grouping pseudo-tokens
   "OPtoks" : print the total # of operator pseudo-tokens
   "TPtoks" : print the total # of total pseudo-tokens
   "All"    : print all of the above totals
   "AESL"   : print the total # of assembly-equivalent source lines

The above keywords may be abbreviated to a unique prefix if desired.
If the B&lt;-counts&gt; option is I&lt;not&gt; specified, then
C&lt;Lines+Blank+Cmnts+NCSL+TPtoks&gt; is implied.

=item B&lt;-sections&gt; I&lt;SPEC&gt;

Specify the sections in the output. I&lt;SPEC&gt; may contain any
of the following (separated by '+'):

   "Header"     : the column labels and separator bar
   "Files"      : the counts for each input file
   "LangTotals" : the totals for each language encountered.
   "Totals"     : the totals for all files.

The above keywords may be abbreviated to a unique prefix
if desired. If the B&lt;-sections&gt; option is I&lt;NOT&gt; specified,
then C&lt;Header+Files+LangTotals+Totals&gt; is implied.

=item B&lt;-vcstring&gt; I&lt;STRING&gt;

Specify that I&lt;STRING&gt; is the delimiter used by your version control
system and that the file suffix should be determined by only
considering the portion of the file path that precedes the first
occurrence of I&lt;STRING&gt;.

=item B&lt;-output&gt; I&lt;FILE&gt;

Redirect output to the named file (should have the same effect as
redirecting STDOUT on the cmdline using "&gt; I&lt;FILE&gt;").

=item I&lt;FILE&gt;

Name of an input file.

=back

Option names are case insensitive and only a unique prefix is required.

=head1 DESCRIPTION

B&lt;sclc&gt; will count the number of lines, blank-lines, comments,
non-comment source lines (NCSL), pseudo-tokens of various types, total
pseudo-tokens, and optionally, assembly equivalent source lines (AESL)
in the given input files (which are assumed to be free of syntax
errors). The output format will look something like the following:

  &lt;header with column titles and separator line&gt;
  &lt;count1&gt; &lt;count2&gt; ... &lt;filename&gt;  (&lt;language&gt;)
    ...      ...           ...         ...
  &lt;total1&gt; &lt;total2&gt; ... ----- &lt;language&gt; -----  (# files)
    ...      ...           ...         ...
  &lt;total1&gt; &lt;total2&gt; ... ***** TOTAL *****  (# files)

If the B&lt;-diff&gt; option is given than two lines are printed for each set of
counts: one for deleted lines and one for inserted lines (and the C&lt;DELETED&gt;
or C&lt;INSERTED&gt; keyword will appear on each output-line). If B&lt;-pred&gt; or
B&lt;-fromversion&gt; or B&lt;-toversion&gt; is given then B&lt;-diff&gt; is implied and B&lt;cdiff&gt;
command is invoked with the B&lt;-blank_ignore&gt; option to compare file versions.

The B&lt;-counts&gt; and B&lt;-sections&gt; options may be used to control which counts
and/or sections to print. The counts are always printed in an order such that:

     lines --&gt; blank-lines --&gt; comments --&gt; NCSL --&gt; &lt;xPtoks&gt;... --&gt; AESL

where "X --&gt; Y" means that the count of Xs always precedes the count of Ys.

=head1 EXAMPLES

The typical use for B&lt;sclc&gt; would be to just give it the filenames on
the command line:

  $ sclc dir.c dir.h
   Lines  Blank  Cmnts   NCSL ... TPtoks     
  ====== ====== ====== ====== ... ======  =============================
     215     26     14    158       1395  dir.c  (C)
      41      8     16     24        160  dir.h  (C)
     256     34     30    182       1555  ----- C -----  (2 files)

There are options to control which columns you do/dont want to see.
The C&lt;NCSL&gt; column is for "non-comment source lines". The C&lt;TPtoks&gt;
column gives the total number of psuedo-tokens of all types.

The optional C&lt;AESL&gt; column is for "assembly equivalent source lines"
(it calculates this using assembly-equivalence factors published by
Software Productivity Research).  

If all you care about is NCSL, you could simply invoke it as:

  $ sclc -counts ncsl dir.[ch]
   NCSL  
  =====  ==============================================================
    158  dir.c  (C)
     24  dir.h  (C)
    182  ----- C -----  (2 files)

If you dont want the header columns, and want to see the overall totals
instead of the totals per programming language (B&lt;sclc&gt; can handle C, C++,
Java, Perl, Assembly, Pascal, and several other languages) then you could
simply say:

  $ sclc -counts ncsl -sections file+totals *.[ch]

    158  dir.c  (C)
     24  dir.h  (C)
    182  ***** TOTAL *****  (2 files)

If you want to know the number of lines of code that have changed
between versions of files, you can use the B&lt;-pred&gt; option, or a
combination of the B&lt;-fromversion&gt; and B&lt;-toversion&gt; options. If
you simply want to know the number of new and changed lines of code
between a set of files and their immediate predecessors, use the B&lt;-pred&gt;
option:

  $ sclc -pred *.[ch]

  Lines  Blank  Cmnts   NCSL   AESL     
  =====  =====  =====  =====   ====  ===============================
      3      1      0      2    5.0  dir.c DELETED  (C)
      5      1      0      4   10.0  dir.c INSERTED  (C)
      1      0      0      1    2.5  dir.h DELETED  (C)
      2      0      0      2    5.0  dir.h INSERTED  (C)
      4      1      0      3    7.5  ----- C DELETED -----  (2 files)
      7      1      0      6   15.0  ----- C INSERTED -----  (2 files)

Again, if you want to limit the output to include only the NCSL counts for
the individual files and their totals, add the B&lt;-counts&gt; and B&lt;-sections&gt;
options into the fold:

  $ sclc -pred -counts ncsl -sections file+totals *.[ch]

      2  dir.c DELETED  (C)
      4  dir.c INSERTED  (C)
      1  dir.h DELETED  (C)
      2  dir.h INSERTED  (C)
      3  ***** TOTAL DELETED *****  (2 files)
      6  ***** TOTAL INSERTED *****  (2 files)

If you want to count the difference in source lines between specific
versions (not just the predecessor) you may use the B&lt;-fromversion&gt;
option to specify the base ("from") version to use for all comparisons.
If the base version is not explicitly specified, it defaults to whatever
version is selected in the users view. Similarly, the B&lt;-toversion&gt;
option may be used to specify the target ("to") version to use for all
comparisons. If the target version is not specified, then (like the base
version) it defaults to whatever version is selected in the users view.

So if I want to count the changed lines of code between what is in my
view and what was on the C&lt;REL1.0.0&gt; label, I would do the following:

  $ sclc -fromver REL1.0.0 *.[ch]

And if I wanted to count the changed lines of code between files on
my C&lt;/main/rel1/mychanges&gt; branch and the REL1.0.0 baseline label they
where branched off from, I would use:

  $ sclc -fromver REL1.0.0 -tover .../mychanges/LATEST *.[ch]

If I wanted to see the difference in lines of code for files on to
different labels I would do something like:

  $ sclc -fromver REL1.0.0 -tover REL2.0.0 *.[ch]

If I wanted to do the same as the above, but my view was already selecting
everything in the REL1.0.0 baseline, then I could simply do:

  $ sclc -tover REL2.0.0 *.[ch]

Hopefully, this "tutorial" should have provided you with a good set of
cookbook examples for using B&lt;sclc&gt;. if you ever want to see this manual
page, you need only invoke B&lt;sclc&gt; using the B&lt;-help&gt; option:

  $ sclc -help

=head1 SEE ALSO

L&lt;cdiff&gt; -- B&lt;sclc&gt; uses B&lt;cdiff&gt; to compare versions of files.

=head1 AUTHOR

Brad Appleton  E&lt;lt&gt;bradapp@computer.orgE&lt;gt&gt;

=cut

## Globals (yuck!)
my %OPTS   = ();
my %CNTS   = ();
my %Totals = ();
my %SECS   = ();
my $ERRORS = 0;
my %KIND   = ();
my $DEFAULT_LANG = "";
my $LANG_NAME    = "";
my $LANG_REF     = "";
my %LANG_RULES   = ();
my $NumSources   = 0;
my %NumSources   = ();

## Count names
my @Count_Names =
    qw(Lines Blank Cmnts NCSL KPtoks SPtoks CPtoks GPtoks OPtoks TPtoks AESL);

##----------------------------------------------------------------------
## DESIGN:
##
## [ Okay listen up and listen good. Here is the strategy we use ... ]
## 
## Basically, foreach language we "know" we only care about the following:
##   - comments (ones that are delimited and ones that continue to end-of-line)
##   - quotations
##   - statement-terminators
##   - block-statement terminators
##   - pseudo-tokens
##
## Therefore for each language, we need to know which characters to pay
## attention to if we are inside a comment, quotations, or neither. Hence
## we will use two arrays for each language:
##
##   1) An array of substitutions to perform any necessary preprocessing
##      for this particular language. We need this to do things like 
##      editing potentially confusing constructs (like replacing the keyword
##      'end' with a single token that is the same regardless of case, or 
##      to deal with things like substitutions and variable names in perl).
##
##   2) An associative with the following keys:
##         $Array{'AESL'} -- Assembly Equivalent multiplier for determining
##                           AESL from NCSL. These values are obtained from
##                           the Programming Language Tables at Capers Jones'
##                           Software Productivity Research, Inc. from the
##                           URL http://www.spr.com/library/langtbl.htm
##
##         $Array{'START_STATE'} -- A regular expression that will match
##            only the tokens I am interested in if I am not inside a
##            comment or a quotation.
##
##         $Array{'CMMNT_STATE'} -- A regular expression that will match
##            only the tokens I am interested in if I am inside a comment
##            whose end is delimited by something other than a newline
##
##         $Array{'CMMNT_NESTING'} -- true or false, depending on whether
##            or not comments are allowed to nest.
##
##         $Array{'&lt;token&gt;QUOTE_STATE'} -- A regular expression that will
##            match only the tokens I am interested in if I am inside a
##            quoted context whose beginning was denoted by &lt;token&gt;.
##
##         $Array{END_&lt;token&gt;} -- The string/character that corresponds to the
##            end of a quoted context whose beginning was denoted by &lt;token&gt;.
##
##         $Array{&lt;token&gt;} -- This should be one of the following predefined
##            constants:
##
my ($EolCmnt,    ## A comment that continues until end-of-line
    $BeginCmnt,  ## Beginning of a delimited comment
    $EndCmnt,    ## End of a delimited comment
    $BeginQuote) ## The beginning of a quoted/literal context
      = (1, 2, 3, 4);
##
##            For each &lt;token&gt; that we will ever match, $Array{&lt;token&gt;}
##            will yield the token type (its semantic meaning).
##

## Need the following array for when the LANG_DEFNS array has to correspond
## to more than one language with a different AESL multiplier for each. The
## Index should be the LANG_NAME.
##
my %AssemblyEquivalence = ('C++', 6.0);

## We use these defaults if no value is specified for a particular language.
## Be wary of changing these defaults, but feel free to override them.
## For reference, all ASCII operators: [~`!\@#\$%^&amp;*_\-+=|\\\\:\'"&lt;&gt;.?/]
my %DEFAULT_DEFNS = ('KEYWORD', '0', # no keywords specified
		     'SYMBOL', '[a-zA-Z]\w*',
		     'NUMBER', '(?=\.?\d)[\d_]*(?:\.[\d_]*)?(?:[a-zA-Z][-+]?\d)?\w*',
		     'GROUPER', '[(){}[\],;]',
		     'OPERATOR', '(.)\1*=?',
		     );
##
##----------------------------------------------------------------------

                    ### BEGIN LANGUAGE DEFINITIONS ###

##----------------------------------------------------------------------
## Assembly language definitions
##   NOTE: these are pretty generic and may not work for all assembly languages
my @ASSEMBLY_EVALS = ('s/^\s*\*/;/');  ## Treat '*' at beginning as a comment
my %ASSEMBLY_DEFNS = ('AESL', 1.0,
                      'START_STATE', ";",
                      'CMMNT_STATE', "",
                      'CMMNT_NESTING', "",
                      ';', $EolCmnt,
                     );
$LANG_RULES{'ASSEMBLY'} = [ \@ASSEMBLY_EVALS, \%ASSEMBLY_DEFNS ];

##----------------------------------------------------------------------
## C/C++ language definitions
my %C_KEYWORDS;
@C_KEYWORDS{			# standard C
    qw(define defined error ifdef if elif else endif include undef
       auto break case char const continue
       default do double else enum extern
       float for goto if int long register return
       short signed sizeof static struct switch
       typedef union unsigned void volatile while)} = undef;
@C_KEYWORDS{			# C++ extensions
    qw(asm bool catch class const_cast
       delete dynamic_cast explicit
       false friend inline mutable
       namespace new operator private
       protected public reinterpret_cast
       static_cast template this throw
       true try typeid typename using
       virtual wchar_t)} = undef;
my @C_EVALS = (
               's/[{};]/ /g  if ($OPTS{delim_ignore})'
              );
my %C_DEFNS = ('AESL', 2.5,
               'START_STATE', '/[*/]|["\']',
               'CMMNT_STATE', '\*/',
               'CMMNT_NESTING', "",
               '"QUOTE_STATE', '\\\\.|"',
               'END_"', '"',
               "'QUOTE_STATE", "\\\\.|'",
               "END_'", "'",
               '"', $BeginQuote,
               "'", $BeginQuote,
               '//', $EolCmnt,
               '/*', $BeginCmnt,
               '*/', $EndCmnt,
	       'KEYWORD', 'exists $C_KEYWORDS{$1}',
	       'SYMBOL', '[\$_a-zA-Z][\$\w]*',
	       'OPERATOR', '-&gt;|(.)\1*=?', # add -&gt;
              );
$LANG_RULES{'C'} = [ \@C_EVALS, \%C_DEFNS ];

##----------------------------------------------------------------------
## Java language definitions
my %JAVA_KEYWORDS;
@JAVA_KEYWORDS{
    qw(abstract boolean break byte case
       catch char class const continue
       default do double else extends
       false final finally float for
       goto if implements import instanceof
       int interface long native new null
       package private protected public
       return short static super switch
       synchronized this throw throws
       transient true try void volatile while)} = undef;
my @JAVA_EVALS = (
                  's/[{};]/ /g  if ($OPTS{delim_ignore})'
                 );
my %JAVA_DEFNS = ('AESL', 6,
                  'START_STATE', '/[*/]|["\']',
                  'CMMNT_STATE', '\\*/',
                  'CMMNT_NESTING', "",
                  '"QUOTE_STATE', '\\\\.|"',
                  'END_"', '"',
                  "'QUOTE_STATE", "\\\\.|'",
                  "END_'", "'",
                  "'", $BeginQuote,
                  '"', $BeginQuote,
                  '//', $EolCmnt,
                  '/*', $BeginCmnt,
                  '*/', $EndCmnt,
		  'KEYWORD', 'exists $JAVA_KEYWORDS{$1}',
		  'SYMBOL', '[\$_a-zA-Z][\$\w]*',
		  );
$LANG_RULES{'JAVA'} = [ \@JAVA_EVALS, \%JAVA_DEFNS ];

##----------------------------------------------------------------------
## Shell language definitions
my %SHELL_KEYWORDS;
@SHELL_KEYWORDS{
    qw(alias case in esac do done od
       eval exec exit export
       for function return
       if test then else fi
       set unset shift shopt trap umask
       while break continue
       until wait)} = undef;
my @SHELL_EVALS = (
                   's/(^|\s+)#/\001/g',   ## denote '#' comments with \001
                   's/\\\\./__/g',        ## remove escaped characters
                   's/[(){}[\];]/ /g  if ($OPTS{delim_ignore})'
                  );
my %SHELL_DEFNS = ('AESL', 15.0,
                   'START_STATE',"[\001`\"']",
                   '"QUOTE_STATE', '\\\\.|"',
                   'END_"', '"',
                   "'QUOTE_STATE", "\\\\.|'",
                   "END_'", "'",
                   "`QUOTE_STATE", "\\\\.|`",
                   "END_`", "`",
                   "\001", $EolCmnt,
                   "'", $BeginQuote,
                   '"', $BeginQuote,
                   '`', $BeginQuote,
		   'KEYWORD', 'exists $SHELL_KEYWORDS{$1}',
		   );
$LANG_RULES{'SHELL'} = [ \@SHELL_EVALS, \%SHELL_DEFNS ];

##----------------------------------------------------------------------
## Make language definitions
my @MAKE_EVALS = (
                  's/^\s*#/\001/g',    ## denote '#' comments with \001
                  's/[(){};]/ /g  if ($OPTS{delim_ignore})'
                 );
my %MAKE_DEFNS = ('AESL', 15.0,
                  'START_STATE',"[\001`\"']",
                  '"QUOTE_STATE', '\\\\.|"',
                  'END_"', '"',
                  "'QUOTE_STATE", "\\\\.|'",
                  "END_'", "'",
                  "`QUOTE_STATE", "\\\\.|`",
                  "END_`", "`",
                  "\001", $EolCmnt,
                  "'", $BeginQuote,
                  '"', $BeginQuote,
                  '`', $BeginQuote,
                 );
$LANG_RULES{'MAKE'} = [ \@MAKE_EVALS, \%MAKE_DEFNS ];

##----------------------------------------------------------------------
## Awk language definitions
my @AWK_EVALS = ('s/(^|\s+)#/\001/g',   ## denote '#' comments with \001
                 's/\\\\./__/g',        ## remove escaped characters
                 's/[{};]/ /g  if ($OPTS{delim_ignore})'
                );
my %AWK_DEFNS = ('AESL', 15.0,
                 'START_STATE',"[\001\"']",
                 '"QUOTE_STATE', '\\\\.|"',
                 'END_"', '"',
                 "'QUOTE_STATE", "\\\\.|'",
                 "END_'", "'",
                 "\001", $EolCmnt,
                 "'", $BeginQuote,
                 '"', $BeginQuote,
                );
$LANG_RULES{'AWK'} = [ \@AWK_EVALS, \%AWK_DEFNS ];

##----------------------------------------------------------------------
## Tcl language definitions
my @TCL_EVALS = ('s/(^|\s+)#/\001/g',   ## denote '#' comments with \001
                 's/\\\\./__/g',        ## remove escaped characters
                 's/[[\]{};]/ /g  if ($OPTS{delim_ignore})'
                );
my %TCL_DEFNS = ('AESL', 5.0,
                 'START_STATE',"[\001`\"']",
                 '"QUOTE_STATE', '\\\\.|"',
                 'END_"', '"',
                 "'QUOTE_STATE", "\\\\.|'",
                 "END_'", "'",
                 "`QUOTE_STATE", "\\\\.|`",
                 "END_`", "`",
                 "\001", $EolCmnt,
                 "'", $BeginQuote,
                 '"', $BeginQuote,
                 '`', $BeginQuote,
                );
$LANG_RULES{'TCL'} = [ \@TCL_EVALS, \%TCL_DEFNS ];

##----------------------------------------------------------------------
## Perl language definitions
##
##    NOTE: I tried to put in some substitutions and definitions to treat
##          stuff between quotes (double and single) and between slashes
##          (for matches *and* substitutions) as literals, but it is too
##          complex to get it all right.
##
##    [A second try, by Bill Mann.]
##    This works best if the perl source follows some rules:
##       Put at least one space between code and comments
##       Backslash-quote #,',",` when used in regexp,q,qq,qx,qw,&lt;&lt;
##
my @PERL_EVALS = ('s/\\\\./__/g;',       ## remove escaped characters
		  's/\$[\'"`#]/\$_/g;',  ## avoid special symbol problems
		  's/(^|\s+)#/\001/g',   ## denote '#' comments with \001
                  's/^=cut/\003/g',      ## end pod comments with \003
                  's/^=[a-z]+/\002/g',   ##   and start them with \002
                  's/[{};]/ /g  if ($OPTS{delim_ignore})'
                );
my %PERL_DEFNS = ('AESL', 15.0,
                  'START_STATE', "['\"`]|[\001\002]",
                  'CMMNT_STATE', "^\003",
                  'CMMNT_NESTING', "",
		  '"QUOTE_STATE', '"',
		  'END_"', '"',
		  "'QUOTE_STATE", "'",
		  "END_'", "'",
		  "`QUOTE_STATE", "`",
		  "END_`", "`",
		  "'", $BeginQuote,
		  '"', $BeginQuote,
		  '`', $BeginQuote,
                  "\001", $EolCmnt,
                  "\002", $BeginCmnt,
                  "\003", $EndCmnt,
		  'KEYWORD', '$1 =~ /^[a-z]/', # any symbol in lowercase
		  'SYMBOL', '(?i)[$@%][^\sa-z]|[$@%&amp;]?[a-z_]\w*',
		  'OPERATOR', '&lt;=&gt;|-&gt;|=&gt;|=~|!~|\?:|(.)\1*=?',
                 );
$LANG_RULES{'PERL'} = [ \@PERL_EVALS, \%PERL_DEFNS ];

##----------------------------------------------------------------------
## Ada language definitions
my @ADA_EVALS = (
                 "s/';'//g",  ## the only single-quote we need to worry about
                 's/\b(?:begin|end|is)\b|;/ /gi  if ($OPTS{delim_ignore})'
                );
my %ADA_DEFNS = ('AESL', 6.5,
                 'START_STATE',"[\"]|--",
                 'CMMNT_STATE', "",
                 'CMMNT_NESTING', "",
                 '"QUOTE_STATE', '""|"',
                 'END_"', '"',
                 "--", $EolCmnt,
                 '"', $BeginQuote,
		 );
$LANG_RULES{'ADA'} = [ \@ADA_EVALS, \%ADA_DEFNS ];

##----------------------------------------------------------------------
## Eiffel language definitions
my @EIFFEL_EVALS = (
                    's/\b(?:end|is)\b|;/ /gi  if ($OPTS{delim_ignore})'
                   );
my %EIFFEL_DEFNS = ('AESL', 15.0,
                    'START_STATE',"[\"']|--",
                    'CMMNT_STATE', "",
                    'CMMNT_NESTING', "",
                    '"QUOTE_STATE', '%"|"',
                    'END_"', '"',
                    "'QUOTE_STATE", '%[%"]|"',
                    "END_'", "'",
                    "'", $BeginQuote,
                    '"', $BeginQuote,
                    "--", $EolCmnt,
		    );
$LANG_RULES{'EIFFEL'} = [ \@EIFFEL_EVALS, \%EIFFEL_DEFNS ];

##----------------------------------------------------------------------
## Pascal language definitions
my @PASCAL_EVALS = (
                    's/\b(?:begin|end)\b|;/ /gi  if ($OPTS{delim_ignore})'
                   );
my %PASCAL_DEFNS = ('AESL', 3.5,
                    'START_STATE',"[']|{|\\(\\*",
                    'CMMNT_STATE', '}|\*\)',
                    'CMMNT_NESTING', 1,
                    "'QUOTE_STATE", "''|'",
                    "END_'", "'",
                    "'", $BeginQuote,
                    '(*', $BeginCmnt,
                    '*)', $EndCmnt,
                    '{', $BeginCmnt,
                    '}', $EndCmnt,
                   );
$LANG_RULES{'PASCAL'} = [ \@PASCAL_EVALS, \%PASCAL_DEFNS ];

##----------------------------------------------------------------------
## Lisp language definitions
my %LISP_KEYWORDS;
@LISP_KEYWORDS{
    qw(and append boundp car catch
       cdr concat cond cons defun defvar
       eq equal error eval fboundp get
       if lambda let list load map mapcar
       member memq nconc nil not or
       prin1 princ prog progn put quote
       reverse set setq t throw unless
       when while zerop)} = undef;
my @LISP_EVALS = (
                  's/[()]/ /g  if ($OPTS{delim_ignore})'
                 );
my %LISP_DEFNS = ('AESL', 5.0,
                  'START_STATE', "[;\"]",
                  'CMMNT_STATE', "",
                  'CMMNT_NESTING', "",
                  '"QUOTE_STATE', '\\\\.|"',
                  'END_"', '"',
                  ";", $EolCmnt,
                  '"', $BeginQuote,
		  'KEYWORD', 'exists $LISP_KEYWORDS{$1}',
                 );
$LANG_RULES{'LISP'} = [ \@LISP_EVALS, \%LISP_DEFNS ];

##----------------------------------------------------------------------
## Matlab/Octave language definitions
## Problems are block_comments %{ %}, ... comments, and the transpose op '
my %MATLAB_KEYWORDS;
@MATLAB_KEYWORDS{
    qw(break case catch continue else elseif end
       false find for function global if otherwise
       persistent return switch true try while)} = undef;
my @MATLAB_EVALS = (
		    's/^\s*%([{}])\s*$/\001$1/',
		    's/(\.\.\.\s*)\S/$1%/',
		    's/(\G|[^.\w\)\}\]\'])\'(.*?)\'/$1\002$2\002/g',
		    's/\bend\b|;/ /g  if ($OPTS{delim_ignore})'
		    );
my %MATLAB_DEFNS = ('AESL', 15,	# who knows ...
		    'START_STATE', "%|\001[{}]|\002",
		    'CMMNT_STATE', "\001[{}]",
		    'CMMNT_NESTING', 1,
		    "\002QUOTE_STATE", "\002\002|\002",
		    "END_\002", "\002",
		    '%', $EolCmnt,
		    "\001{", $BeginCmnt,
		    "\001}", $EndCmnt,
		    "\002", $BeginQuote,
		    'KEYWORD', 'exists $MATLAB_KEYWORDS{$1}',
		    'OPERATOR', '\.[\'*\\\\^/]|(.)\1*=?', # add dot operators
		    );
$LANG_RULES{'MATLAB'} = [ \@MATLAB_EVALS, \%MATLAB_DEFNS ];

##----------------------------------------------------------------------
## ZPL language definitions
my %ZPL_KEYWORDS;
@ZPL_KEYWORDS{
    qw(array at band begin block boolean
       bor bread bwrite bxor by char
       complex config const constant continue
       cyclic dcomplex direction distribution
       do double downto else elsif end exit
       extern file float for free generic
       genericensemble grid halt if in inout
       integer interleave irregular longint
       max min multiblock nondist of opaque
       out procedure program prototype qcomplex
       quad read record reflect region repeat
       return sbyte scaledby scan shortint
       static string swith swithout then to
       type ubyte uinteger ulongint union until
       ushortint var while with without wrap
       write writeln)} = undef;
my @ZPL_EVALS = (
		 's/\b(?:begin|end)\b|;/ /g  if ($OPTS{delim_ignore})'
		 );
my %ZPL_DEFNS = ('AESL', 15,	# who knows ...
		 'START_STATE', '/\*|--|["\']',
		 'CMMNT_STATE', '\*/',
		 'CMMNT_NESTING', "",
		 '"QUOTE_STATE', '\\\\.|"',
		 'END_"', '"',
		 "'QUOTE_STATE", "\\\\.|'",
		 "END_'", "'",
		 '"', $BeginQuote,
		 "'", $BeginQuote,
		 '--', $EolCmnt,
		 '/*', $BeginCmnt,
		 '*/', $EndCmnt,
		 'KEYWORD', 'exists $ZPL_KEYWORDS{$1}',
		 'SYMBOL', '[_a-zA-Z]\w*',
		 'OPERATOR', '@\^|(.)\1?=?', # add @^, break up |||
		 );
$LANG_RULES{'ZPL'} = [ \@ZPL_EVALS, \%ZPL_DEFNS ];

##----------------------------------------------------------------------
## FORTRAN language definitions
my %FORTRAN_KEYWORDS;
@FORTRAN_KEYWORDS{
    qw(allocatable allocate assign automatic
       backspace block byte call case character
       close common complex contains continue cycle
       data deallocate dimension do dowhile double
       else elseif elsewhere end enddo endif endselect
       endtype entry equivalence exit external
       forall format function go goto if implicit
       integer intent interface intrinsic logical
       module namelist nullify open optional
       parameter pause pointer precision print
       private procedure program protected public
       read real record return rewind
       save select sequence static stop subroutine
       target to type use value virtual volatile
       wait where while write)} = undef;
my @FORTRAN_EVALS =
              ('s/^[c\\*]/!/gi', ## denote 'c ...' comments with !
               's/^\s*end[^!\n]*/ /gi  if ($OPTS{delim_ignore})'
              );
my %FORTRAN_DEFNS = ('AESL', 5,	# who knows ...
		     'START_STATE', "!|[\"']",
		     'CMMNT_STATE', "",
		     'CMMNT_NESTING', "",
		     '"QUOTE_STATE', '\\\\.|"',
		     'END_"', '"',
		     "'QUOTE_STATE", "\\\\.|'",
		     "END_'", "'",
		     '"', $BeginQuote,
		     "'", $BeginQuote,
		     '!', $EolCmnt,
		     '/*', $BeginCmnt,
		     '*/', $EndCmnt,
		     'KEYWORD', 'exists $FORTRAN_KEYWORDS{$1}',
		     );
$LANG_RULES{'FORTRAN'} = [ \@FORTRAN_EVALS, \%FORTRAN_DEFNS ];

##----------------------------------------------------------------------
## Chapel language definitions
my %CHAPEL_KEYWORDS;
@CHAPEL_KEYWORDS{
    qw(allocate and array atomic boolean break call
       class cobegin complex config const constructor
       continue distribute do domain enum
       except expect false float for forall function
       goto if implements in index inout integer
       invariant iterator let like local Locales
       max min module nil not of on or otherwise
       out parameter private public
       release repeat return select seq subtype
       to true type typeselect union until
       unordered var view when where
       while with yield)} = undef;
my @CHAPEL_EVALS = (
		    's/[{};]/ /g  if ($OPTS{delim_ignore})'
		    );
my %CHAPEL_DEFNS = ('AESL', 15,	# who knows ...
		    'START_STATE', '/\*|--|//|["\']',
		    'CMMNT_STATE', '/\*|\*/',
		    'CMMNT_NESTING', 1,
		    '"QUOTE_STATE', '\\\\.|"',
		    'END_"', '"',
		    "'QUOTE_STATE", "\\\\.|'",
		    "END_'", "'",
		    '"', $BeginQuote,
		    "'", $BeginQuote,
		    '--', $EolCmnt,
		    '//', $EolCmnt,
		    '/*', $BeginCmnt,
		    '*/', $EndCmnt,
		    'KEYWORD', 'exists $CHAPEL_KEYWORDS{$1}',
		    'SYMBOL', '[\$_a-zA-Z][?\$\w]*',
		    'NUMBER', '\.?\d[.\w]*',
		    'GROUPER', '[(){}[\],;]',
		    );
$LANG_RULES{'CHAPEL'} = [ \@CHAPEL_EVALS, \%CHAPEL_DEFNS ];

                    ### END OF LANGUAGE DEFINITIONS ###

## Default language extensions
##----------------------------------------------------------------------
## GLOBAL:
##
## DESCRIPTION:
##
##----------------------------------------------------------------------
my %Suffixes = (
   ## Assembler
      ".s",   "Assembly",
      ".S",   "Assembly",
      ".as",  "Assembly",
      ".AS",  "Assembly",
      ".asm", "Assembly",
      ".ASM", "Assembly",
   ## C
      ".c", "C",
      ".C", "C",  ## We have some special rules we use later on so we
      ".h", "C",  ## don't mistake a C++ file for a C file.
      ".H", "C",
   ## C++
      ".cc",  "C++",
      ".cxx", "C++",
      ".cpp", "C++",
      ".hh",  "C++",
   ## Chapel
      ".chpl", "Chapel",
   ## Ada
      ".a",   "Ada",
      ".ada", "Ada",
      ".Ada", "Ada",
      ".ADA", "Ada",
   ## Eiffel
      ".e",   "Eiffel",
      ".efl", "Eiffel",
      ".Efl", "Eiffel",
      ".EFL", "Eiffel",
   ## FORTAN
      ".f",   "FORTRAN",
      ".for", "FORTRAN",
      ".f90", "FORTRAN",
      ".F",   "FORTRAN",
      ".F90", "FORTRAN",
   ## Java
      ".java",  "Java",
      ".jav",   "Java",
      ".JAV",   "Java",
   ## Lisp
      ".l",   "Lisp",
      ".lsp", "Lisp",
      ".Lsp", "Lisp",
      ".LSP", "Lisp",
   ## Matlab
      ".m",   "Matlab",
   ## Pascal
      ".p",   "Pascal",
      ".pas", "Pascal",
      ".Pas", "Pascal",
      ".PAS", "Pascal",
   ## Perl
      ".pl",  "Perl",
      ".PL",  "Perl",
      ".plt", "Perl",
      ".PLT", "Perl",
      ".ph",  "Perl",
      ".PH",  "Perl",
      ".pm",  "Perl",
      ".PM",  "Perl",
   ## shell
      ".sh",   "shell",
      ".bash", "shell",
      ".csh",  "shell",
      ".ksh",  "shell",
      ".tcsh", "shell",
      ".zsh",  "shell",
   ## make
      ".mk",        "make",
      ".MK",        "make",
      ".make",      "make",
      ".MAKE",      "make",
   ## Awk
      ".awk",  "Awk",
      ".AWK",  "Awk",
   ## Tcl
      ".tcl",   "Tcl",
      ".Tcl",   "Tcl",
      ".TCL",   "Tcl",
      ".tclsh", "Tcl",
      ".TCLSH", "Tcl",
      ".tk",    "Tcl",
      ".Tk",    "Tcl",
      ".TK",    "Tcl",
      ".wish",  "Tcl",
      ".WISH",  "Tcl",
   ## ZPL
      ".z",     "ZPL",
      ".zh",    "ZPL",
);

## Default language interpreters
##----------------------------------------------------------------------
## GLOBAL:
##
## DESCRIPTION:
##
##----------------------------------------------------------------------
my %Interpreters = (
   ## Perl
      "per5", "Perl",
      "perl", "Perl",
      "Perl", "Perl",
      "PERL", "Perl",
   ## Awk
      "awk",  "Awk",
   ## shell
      "sh",   "shell",
      "bash", "shell",
      "csh",  "shell",
      "ksh",  "shell",
      "tcsh", "shell",
      "zsh",  "shell",
   ## Tcl
      "tcl",   "Tcl",
      "Tcl",   "Tcl",
      "TCL",   "Tcl",
      "tclsh", "Tcl",
      "TCLSH", "Tcl",
      "tk",    "Tcl",
      "Tk",    "Tcl",
      "TK",    "Tcl",
      "wish",  "Tcl",
      "WISH",  "Tcl"
);


##----------------------------------------------------------------------
## FUNCTION:
##   ErrorMsg -- Print an error message (prefixed by "$NAME: ").
##
## SYNOPSIS:
##   &amp;ErrorMsg("message text");
##
## ARGUMENTS:
##   A single string containing the message text.
##
## RETURN VALUE: None.
##
## PRECONDITIONS:
##   The global variable $NAME should be set to the name of the running
##   script.
##
## SIDE EFFECTS:
##   - Prints to STDERR
##----------------------------------------------------------------------
sub ErrorMsg {
    print STDERR ("${NAME}: ", @_, "\n");
}

##----------------------------------------------------------------------
## FUNCTION:
##   FatalMsg -- Print an error message and then abort
##
## SYNOPSIS:
##   &amp;FatalMsg("message text");
##
## ARGUMENTS:
##   A single string containing the message text.
##
## RETURN VALUE: None.
##
## PRECONDITIONS:
##   The global variable $NAME should be set to the name of the running
##   script.
##
## SIDE EFFECTS:
##   - Prints an error message
##   - Exits the script using exit(2)
##----------------------------------------------------------------------
sub FatalMsg {
    &amp;ErrorMsg(@_);
    exit(2);
}

##----------------------------------------------------------------------
## FUNCTION:
##   Usage -- Print a Usage message and then exit with the specified
##            exit-value. If the exit-value is &gt; 1, then usage is terse
##            (synopsis only) and goes to STDERR. Otherwise, usage is
##            verbose and goes to STDOUT.
##
## SYNOPSIS:
##   &amp;Usage([$val]);
##
## ARGUMENTS:
##   $val : (optional) The integer exit value to use (defaults to 2).
##
## RETURN VALUE: None.
##
## PRECONDITIONS: None.
##
## SIDE EFFECTS:
##   Exits the script using exit with the given exit-status.
##----------------------------------------------------------------------
sub Usage {
    local($_) = shift;
    my ($exitval, $message) = (1, '');
    ## Collect arguments
    if ( /^[-+]?\d+$/ ) {
	($exitval, $message) = ($_, '');
    }
    elsif (defined $_  and  length $_) {
	($exitval, $message) = (2, $_);
    }
    printf STDERR ("%s: %s\n", basename($0), $message)  if ($message);
    $ENV{PATH} .= $PathList_Sep . $Config{bin};
    system( $Config{bin} . $Path_Sep . 'perldoc', $0 );
    exit($exitval);
}

##----------------------------------------------------------------------
## FUNCTION:
##   DbgPrintf -- Print debugging output. Prints the specified arguments
##                only if '-debug' was specified on the command-line.
##
## SYNOPSIS:
##   &amp;DbgPrintf("%s\n", "debug message text");
##
## ARGUMENTS:
##   The format and variables containing the debug message text (a newline
##   is not automatically printed so include it yourself if you need it).
##
## RETURN VALUE: None.
##
## PRECONDITIONS:
##   Assumes that the global variable named $OPTS{'debug'} will evaluate to
##   TRUE if '-debug' was given on the command-line.
##
## SIDE EFFECTS:
##   Prints to STDERR
##----------------------------------------------------------------------
sub DbgPrintf {
    printf STDERR (@_) if ($OPTS{'debug'});
}

## Match a string against a set of keywords (case insensitive and
## allowing unique prefixes). Return the list of matching elements.
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub MatchKwd {
    my($str, @keywords) = @_;
    local $_;
    my @matches = grep( m/^${str}$/i, @keywords );
    @matches = grep( m/^${str}/i, @keywords ) if ( @matches == 0 );
    @matches;
}

## Validate -language option
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub CheckLanguage {
    my(@extspecs) = @_;
    local($_);
    foreach (@extspecs) {
	my ($ext, $lang) = ('', $_);
	($ext, $lang) = ($1, $2) if (/^([^=]*)=(.+)$/);
	if (! $lang) {
	    &amp;ErrorMsg("invalid -language argument \"$_\"");
	    ++$ERRORS;
	    next;
	}
	my $expr = $lang;
	$expr =~ s/\W/\\$&amp;/g;
	my @matches = grep(/^${expr}$/i, @Languages);
	if (@matches == 0) {
	    &amp;ErrorMsg("Unknown programming language: \"$lang\"");
	    ++$ERRORS;
	}
	$lang = $matches[$[];
	if (! $ext) {
	    $DEFAULT_LANG = $lang;
	}
	elsif ($ext =~ m|^/(.*)$|) {
	    $Interpreters{$1} = $lang;
	}
	else {
	    $Suffixes{$ext} = $lang;
	}
    }
}

## Validate -sections option
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub CheckSections {
    local($_);
    my @secspecs = split(/\W/, $OPTS{'sections'});
    if (0 == @secspecs) {
	&amp;ErrorMsg("invalid sections specifier \"$OPTS{'sections'}\"");
	++$ERRORS;
    }
    else {
	my @matches = ();
	foreach (@secspecs) {
	    @matches = &amp;MatchKwd($_, "header", "files", "langtotals", "totals");
	    if (@matches == 1) {
		++$SECS{$matches[0]};
	    }
	    elsif (@matches == 0) {
		&amp;ErrorMsg("invalid sections specifier \"$_\"");
		++$ERRORS;
	    }
	    else {
		&amp;ErrorMsg("ambiguous sections specifier \"$_\"");
		++$ERRORS;
	    }
	}
    }
}

## Validate -counts option
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------

sub CheckCounts {
    local($_);
    my @cntspecs = split(/\W/, $OPTS{'counts'});
    if (0 == @cntspecs) {
	&amp;ErrorMsg("invalid counts specifier \"$OPTS{'counts'}\"");
	++$ERRORS;
    }
    else {
	my @matches = ();
	foreach (@cntspecs) {
	    @matches = &amp;MatchKwd($_, @Count_Names, 'all');
	    if (@matches == 1) {
		if ($matches[0] eq 'all') {
		    for (@Count_Names) {
			next if $_ eq 'AESL'; # skip this one
			++$CNTS{lc $_};
		    }
		} else {
		    ++$CNTS{lc $matches[0]};
		}
	    }
	    elsif (@matches == 0) {
		&amp;ErrorMsg("invalid counts specifier \"$_\"");
		++$ERRORS;
	    }
	    else {
		&amp;ErrorMsg("ambiguous counts specifier \"$_\"");
		++$ERRORS;
	    }
	}
    }
}

##----------------------------------------------------------------------
## FUNCTION:
##   SplitPath -- Split a pathname into dirname and basename components
##
## SYNOPSIS:
##   ($dirname, $basename, $vcsuffix) = &amp;SplitPath($pname);
##
## ARGUMENTS:
##   $pname : The pathname of a file
##
## RETURN VALUE:
##   Returns the array ($dirname, $basename, $vcsuffix)
##   where $vcsuffix is the portion of the string (if any) the follows
##   the first occurrence of $OPTS{'vcstring'}
##
## PRECONDITIONS:
##   Assumes the global $OPTS{'vcstring'} is the version-control systems
##   separator text for splitting a pathname from its version.
##
## SIDE EFFECTS: None.
##----------------------------------------------------------------------
sub SplitPath {
    local($_) = @_;
    my($dirname, $basename, $vcsuffix, $i);
    if ($OPTS{'vcstring'}) {
	$i = index($_, $OPTS{'vcstring'});
	unless ($i &lt; 0) {
	    $vcsuffix = substr($_, $i+1);
	    $_ = substr($_, 0, $i);
	}
    }
    ($dirname, $basename) = ('.', $_);
    $i = rindex($_, $Path_Sep);
    unless ($i &lt; 0) {
	$dirname = substr($_, 0, $i);
	$basename = substr($_, $i+1);
    }
    ($dirname, $basename, $vcsuffix);
}

##----------------------------------------------------------------------
## FUNCTION:
##   ExcludeFile -- See if a file should be excluded from processing.
##
## SYNOPSIS:
##   if (&amp;ExcludeFile($pname)) { ... }
##
## ARGUMENTS:
##   $pname : The pathname of a file that we may wish to process.
##
## RETURN VALUE:
##   TRUE if the file should be excluded from processing,
##   FALSE otherwise.
##
## PRECONDITIONS:
##   Uses the following globals:
##      $OPTS{'except'} -- list of regexps matching files to exclude
##      $OPTS{'name'} -- list of regexps matching files to include
##
## SIDE EFFECTS: None.
##----------------------------------------------------------------------
sub ExcludeFile {
    local($_) = @_;
    ## see if we should exclude this file from the list
    if ((@{ $OPTS{'name'} } &gt; 0) || (@{ $OPTS{'except'} } &gt; 0)) {
	my($dirname, $basename) = &amp;SplitPath($_);
	my @matches = grep($basename =~ /^$_$/, @{ $OPTS{'except'} });
	return 1  if (@matches &gt; 0); ## this is an exception
	if (@{ $OPTS{'name'} } &gt; 0) {
	    ## only accept if we have a match
	    @matches = grep($basename =~ /^$_$/, @{ $OPTS{'name'} });
	    return 1  if (@matches == 0); ## no match
	}
    }
    0;
}

## Return the reference name for the language-definition arrays
## of this language.
##
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub RefLanguage {
    my($lang) = @_;
    local($_) = $lang;
    ## Figure out the language-specific pair of arrays to use
    if ($lang) {
	$_ = "C" if ($lang eq "C++"); ## C &amp; C++ use the same arrays
	## Arrays are named &lt;LANG&gt;_DEFNS and &lt;LANG&gt;_EVALS where &lt;LANG&gt; is
	## all uppercase with non-alphanumerics removed
	s/\W//g;
	s/[a-z]/\U$&amp;/g;
    }
    $_;
}

## Guess the language based on the contents of the given text line
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub GuessLanguage {
    local($_) = @_;
    my($lang) = "";
    if ($DEFAULT_LANG) {
	$lang = $DEFAULT_LANG;
    }
    elsif (m/^\s*\#\s*(include|if|ifdef|define)\b/) {
	$lang = "C";
    }
    elsif (m|^\s*/\*\*\s*$|) {
	$lang = "Java";
    }
    elsif (m|^\s*/\*|) {
	$lang = "C";
    }
    elsif (m|^\s*//|) {
	$lang = "C++";
    }
    elsif (/^\s*\(\*/) {
	$lang = "Pascal";
    }
    elsif (/^\s*\{/) {
	$lang = "Pascal";
    }
    elsif (/^\s*--/) {
	$lang = "Ada";	       ## or it could be Eiffel, ZPL or Chapel
    }
    elsif (/^\s*[\(;]/) {
	$lang = "Lisp";
    }
    elsif (/^\s*\*/) {
	$lang = "Assembly";
    }
    elsif (m:^\s*\#\s*\!\s*\S+$Path_Sep(\S+):) {
	my $execname = $1;	## get the name of the program to exec
	## Remove the extension if we can't find its name
	$execname =~ s/\.[^.]*$//   unless (defined $Interpreters{$execname});
	## Remove trailing numbers if we still can't find its name
	$execname =~ s/\d[.\d]*$//  unless (defined $Interpreters{$execname});
	$lang = $Interpreters{$execname} if (defined $Interpreters{$execname});
    }
    elsif (/^\s*#/) {
	   $lang = "shell";
       }

    $lang = $DEFAULT_LANG if ($DEFAULT_LANG &amp;&amp; (! $lang));

    my $ref = &amp;RefLanguage($lang);
    ($LANG_NAME, $LANG_REF) = ($lang, $ref);
}

## Determine the source language based on the filename
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub SetLanguage {
    my ($filepath) = @_;
    local($_) = $filepath;
    my $lang = "";
    my ($dirname, $basename) = &amp;SplitPath($_);
    ## First see if we have a '.' suffix
    if ($basename =~ /.+(\.[^.]+)$/) {
	$lang = $Suffixes{$1} if (defined $Suffixes{$1});
    }
    elsif ($basename =~ /^makefile/i) {
	$lang = "make";
    }
    elsif ($filepath ne '-') {
	## Not a normal '.' suffix, have to look at all of them
	my %seen = ();
	foreach (keys %Suffixes) {
	    next unless ($_);
	    next if (defined $seen{$_});
	    ++$seen{$_};
	    s/\W/\\$&amp;/g;	## escape regexp meta-characters
	    $lang = $Suffixes{$_} if ($basename =~ m|^.+$_$|);
	    last if ($lang);
	}
    }
    if ($lang) {
	$lang = "C++" if (($lang eq "C") &amp;&amp; ($DEFAULT_LANG eq "C++"));
	my $ref = &amp;RefLanguage($lang) || "";
	($LANG_NAME, $LANG_REF) = ($lang, $ref);
    }
    else {
	## If we still dont know the language -- see if we can tell by
	## looking at the contents of the first non-blank line
	if ($filepath eq '-') {
	    $_ = '';
	}
	elsif (open(FILEPATH, $filepath)) {
	    while (&lt;FILEPATH&gt;) {
		chomp;
		last unless (/^\s*$/);
	    }
	    close(FILEPATH);
	}
	&amp;GuessLanguage($_);
    }
}

## See if the given line matches a -separate expression from the command-line
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub IsSeparator {
    local($_) = @_;
    return 0 if (@{ $OPTS{'separate'} } == 0);
    my $regexp;
    foreach $regexp (@{ $OPTS{'separate'} }) {
	return 1 if (/^(?:$regexp)$/);
    }
    0;
}

## Token -- Count various kinds of token; optionally list them.
##----------------------------------------------------------------------
sub Token {
    my ($counts, $dtype, $ttype, $token) = @_;
    my $tok = $dtype.$ttype;
    my $n = ++$$counts{$tok};
    ++$$counts{$dtype.'TPtoks'};
    DbgPrintf("%4d %s  %s\n", $n, $tok, $token);
}

## SourceCount -- Count the lines, blank-lines, comments, NCSL, tokens, etc.
##                in the given source file (a filename of '-' means use STDIN).
##
## returns the array %counts upon success,
## If the file can't be opened, then returns ('Error', "Error")
##
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
use vars qw(@langEvals %langDefns);

sub SourceCount {
    my $filename = shift;
    local(*langEvals, *langDefns) = @_;
    my %error = ('Error', "Error");
    my $filehandle;
    local($_);
    my $diffType;		## one of '' or '&lt;' or '&gt;'
    my ($inComment, $inQuote) = (0, '');
    my %counts = ();

    ## Fill in any missing langDefns
    for (keys %DEFAULT_DEFNS) {
	$langDefns{$_} = $DEFAULT_DEFNS{$_} unless exists $langDefns{$_};
    }

    ## Open input
    if (($filename eq "-") &amp;&amp; !($OPTS{'filelist'} &amp;&amp; (@ARGV == 0))) {
	$filehandle = \*STDIN;
    }
    else {
	if ($OPTS{'pred'} || $OPTS{'fromversion'} || $OPTS{'toversion'}) {
	    ## invoke cdiff using ClearCase version selectors
	    my $from_pname = $filename;
	    my $to_pname   = $filename;
	    $from_pname      .= "\@\@$OPTS{'fromversion'}"  if ($OPTS{'fromversion'});
	    $to_pname        .= "\@\@$OPTS{'toversion'}"    if ($OPTS{'toversion'});
	    my $cdiff_cmd  = ${EXECDIR} . $Path_Sep . 'cdiff';
	    $cdiff_cmd       .= " -unlimited -noprolog -blank_ignore";
	    $cdiff_cmd       .= ($OPTS{'pred'}) ? " -pred" : " $from_pname";
	    $cdiff_cmd       .= " $to_pname";
	    $filename = "$cdiff_cmd |";
	}
	$filehandle = \*INPUT;
	unless (open(INPUT, $filename)) {
	    &amp;ErrorMsg("Can't open $filename for reading: $!");
	    return %error;
	}
    }

    while (&lt;$filehandle&gt;) {
	chomp;
	$diffType = '';
	## See if this is a context separator.
	if (&amp;IsSeparator($_)) {
	    ## Yes, reset context and get the next line.
	    ($inComment, $inQuote) = (0, '');
	    next;
	}
	## Strip any leading prefix
	s/^(?:$OPTS{'prefix'})// if ($OPTS{'prefix'});

	## If this is diff(1) output, do some special stuff
	if ($OPTS{'diff'}) {
	    if (/^([&lt;&gt;])\s+/) {
		## line was inserted or deleted (strip the prefix)
		$diffType = $1;
		$_ = $'; #'
	    }
	    else {
		## Reset context and get the next line.
		($inComment, $inQuote) = (0, '');
		next;
	    }
	}

	## Check for a blank line
	++$counts{"${diffType}Lines"};
	++$counts{"Lines"} if ($diffType); ## count total lines in a diff
	if (/^\s*$/) {
	    ++$counts{"${diffType}Blank"};
	    next;
	} 

	## The following is a KLUGE to distinguish C++ from C
	if ($LANG_NAME eq "C" &amp;&amp; !($inComment || $inQuote)) {
	    $LANG_NAME = "C++"
		if m/(?: ^\s*
		      (?: \:\:\s*\w |
		       (?: class |
			public \s* : |
			private \s* : |
			protected \s* : |
			template \s* &lt; | 
			virtual | 
			throw \s* [\(;] |
			try \s* [{}] |
			catch \s* \( |
			dynamic_cast
			)
		       )
		      \b) |
		      (?: ^[^\/\"\']*
		       (?: \b
			(?: throw \s* [\(;] |
			 try \s* [{}] |
			 catch \s* \( |
			 dynamic_cast
			 )\b |
			\:\:
			)
		       )/x;
	}

	## Perform any language-specific preprocessing
	my $expr;
	foreach $expr (@langEvals) {
	    eval $expr;
	}
	## See how many comments, statements &amp; tokens are on this line
	my ($hasSource, $hasComment) = (0, 0);
	s/^\s*//;
	while ($_) {
	    if ($inComment &gt; 0) {
		++$hasComment;
		if (m/$langDefns{'CMMNT_STATE'}/) {
		    $_ = $'; #'
		    if ($langDefns{'CMMNT_NESTING'}) {
			--$inComment if ($langDefns{$&amp;} eq $EndCmnt);
			++$inComment if ($langDefns{$&amp;} eq $BeginCmnt);
		    }
		    else {
			$inComment = 0  if ($langDefns{$&amp;} eq $EndCmnt);
		    }
		}
		else {
		    $_ = '';
		}
	    }
	    elsif ($inQuote) {
		++$hasSource;
		if (m/$langDefns{"${inQuote}QUOTE_STATE"}/) {
		    $_ = $'; #'
		    $inQuote = '' if ($&amp; eq $langDefns{"END_${inQuote}"});
		}
		else {
		    $_ = '';
		}
	    }
	    elsif (m/^(?:$langDefns{'START_STATE'})/) {
		$_ = $'; #'
		if ($langDefns{$&amp;} eq $EolCmnt) {
		    ++$hasComment;
		    ++$hasSource if ($` =~ /\S/);
		    $_ = '';
		}
		elsif ($langDefns{$&amp;} eq $BeginCmnt) {
		    ++$hasComment;
		    ++$hasSource if ($` =~ /\S/);
		    $inComment = 1;
		}
		else {
		    ++$hasSource;
		    if ($langDefns{$&amp;} eq $BeginQuote) {
			$inQuote = $&amp;;
			my $quote = $_;
			$quote =~ s/$inQuote.*// or $quote .= '...';
			&amp;Token(\%counts, $diffType, 'CPtoks',
			       "$inQuote$quote$inQuote");
		    }
		}
	    }
	    else {
		++$hasSource;
		if (s/^($langDefns{'SYMBOL'})\s*//) {
		    if (eval $langDefns{'KEYWORD'}) {
			&amp;Token(\%counts, $diffType, 'KPtoks', $&amp;);
		    } else {
			&amp;Token(\%counts, $diffType, 'SPtoks', $&amp;);
		    }
		} elsif (s/^(?:$langDefns{'NUMBER'})\s*//) {
		    &amp;Token(\%counts, $diffType, 'CPtoks', $&amp;);
		} elsif (s/^(?:$langDefns{'GROUPER'})\s*//) {
		    &amp;Token(\%counts, $diffType, 'GPtoks', $&amp;);
		} elsif (s/^(?:$langDefns{'OPERATOR'})\s*//) {
		    &amp;Token(\%counts, $diffType, 'OPtoks', $&amp;);
		} else {	# ignore anything else
		    &amp;ErrorMsg("undefined token '$_'\n");
		    s/^.//;
		}
	    }
	    s/^\s*//;
	}
	++$counts{"${diffType}NCSL"}  if ($hasSource);
	++$counts{"${diffType}Cmnts"} if ($hasComment);
    }

    close($filehandle) unless ($filename eq '-');

    ## Determine Assembly Eqivalent counts
    my $aeFactor = $AssemblyEquivalence{$LANG_NAME};
    $aeFactor = $langDefns{'AESL'} unless ($aeFactor);
    $counts{'AESL'}  = ($counts{'NCSL'}  * $aeFactor) if ($counts{'NCSL'});
    $counts{'&lt;AESL'} = ($counts{'&lt;NCSL'} * $aeFactor) if ($counts{'&lt;NCSL'});
    $counts{'&gt;AESL'} = ($counts{'&gt;NCSL'} * $aeFactor) if ($counts{'&gt;NCSL'});

    %counts;
}

## Print a report line or header. We know it is a header if
## the first argument is a 's', not a 'd'
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub PrintCounts {
    my ($ntype, $fname, $language, $Name, $type, $Count) = @_;
    local $_;
    my ($fwidth, @nums);
    my $ll = 79;
    my $nwidth = 6;

    foreach (@$Name) {
	my $set = $CNTS{lc $_};
	next unless ($set);

	if ($ntype eq 'd') {
	    printf("%${nwidth}${ntype} ", $$Count{"$type$_"} || 0);
	} else {
	    printf("%${nwidth}${ntype} ", $_);
	}
	push(@nums, $_);
    }
    print "${fname}";
    print "  ($language)" if ($language);
    print "\n";

    if ($ntype eq 's') {
	## need to print a header-bar
	my $fwidth = $ll;
	foreach (@nums) {
	    printf("%-${nwidth}s ", '=' x $nwidth);
	    $fwidth -= ($nwidth + 1);
	}
	my $fbar = '=' x $fwidth;
	print "${fbar}\n";
    }
}

##----------------------------------------------------------------------
## FUNCTION:
##   ExpandDirectory -- Expand a directory into all its elements
##
## SYNOPSIS:
##   @elements = &amp;ExpandDirectory($dir);
##
## ARGUMENTS:
##   $dir : The pathname of a directory
##
## RETURN VALUE:
##   An array containing the pathname of all elements in the directory
##   (NOT including '.' or '..').
##
## PRECONDITIONS:
##   Uses the global %TRAVERSED to keep track of which directories
##   have already been expanded (so we dont get into an infinite
##   directory exandning loop).
##
## SIDE EFFECTS:
##   Records the traversed directory in %TRAVERSED.
##----------------------------------------------------------------------
my %TRAVERSED = ();
sub ExpandDirectory {
    my($dir) = @_;
    my($absdir, @elements);
    local($_);
    unless ($Is_WinDos) {
	## On Unix, make sure we dont traverse this directory twice
	## due to symlinks
	my($absdir);
	chomp($absdir = `cd $dir &amp;&amp; /bin/pwd`);
	return @elements if (defined $TRAVERSED{$absdir});
	++$TRAVERSED{$absdir};
    }
    ## Open the directory for reading
    unless (opendir(DIRECTORY, $dir)) {
	&amp;error_msg("Can't traverse directory $dir: $!");
	return @elements;
    }
    $dir = '' if ($dir eq $Path_Sep); ## might be root
    ## Read all the directory entries
    while ($_ = readdir(DIRECTORY)) {
	next if (/^\.*$/);	## ignore . and ..
	push(@elements, "${dir}${Path_Sep}$_");
    }
    closedir(DIRECTORY);
    (@elements);
}

##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub CountFiles {
    my @files = @_;
    local ($_);
    foreach (@files) {
	if (-d $_  &amp;&amp; $OPTS{'recurse'}) {
	    my @entries = &amp;ExpandDirectory($_);
	    &amp;CountFiles(@entries);
	    next;
	}
	## See if we should exclude this file from the list
	next if (&amp;ExcludeFile($_));
	if (-d $_  &amp;&amp; $OPTS{'recurse'}) {
	    &amp;ErrorMsg("$_ is a directory.") unless ($OPTS{'ignore'});
	    next;
	}
	if (-B _) {
	    &amp;ErrorMsg("$_ is a binary file.") unless ($OPTS{'ignore'});
	    next;
	}
	## See if we can determine the programming language
	($LANG_NAME, $LANG_REF) = ('', '');
	&amp;SetLanguage($_);
	&amp;ErrorMsg("Can't determine programming language for $_")
	    unless ($LANG_REF || $OPTS{'ignore'});
	next unless (exists $LANG_RULES{$LANG_REF});
	my %counts = &amp;SourceCount($_, @{ $LANG_RULES{$LANG_REF} });
	next if ($counts{'Error'} || !$counts{'Lines'});
	&amp;PrintCounts('s', '', '', \@Count_Names, "", \%counts)
	    if ((++$NumSources == 1) &amp;&amp; $SECS{'header'});
	++$NumSources{$LANG_NAME};
	my (@kindList, $kind) = ("");
	@kindList = ('&lt;', '&gt;') if ($OPTS{'diff'});
	for $kind (@kindList) {
	    &amp;PrintCounts('d', "$_$KIND{$kind}", $LANG_NAME, \@Count_Names,
			 $kind, \%counts);
	}

	for (keys %counts) {
	    $Totals{$_} += $counts{$_};
	    $Totals{$LANG_NAME}{$_} += $counts{$_};
	}
    }
}

## Report the totals for each language and overall
##----------------------------------------------------------------------
## FUNCTION:
##
## SYNOPSIS:
##
## ARGUMENTS:
##
## RETURN VALUE:
##
## PRECONDITIONS:
##
## SIDE EFFECTS:
##----------------------------------------------------------------------
sub PrintTotals {
    local $_;
    my ($totals) = @_;
    my $str;
    my @languages = (keys %NumSources);
    my (@kindList, $kind) = ("");
    @kindList = ('&lt;', '&gt;') if ($OPTS{'diff'});

    foreach (@languages) {
	for $kind (@kindList) {
	    if ($SECS{'langtotals'}) {
		$str = sprintf("%s  (%d %s)",
			       "----- $_$KIND{$kind} -----",
			       $NumSources{$_},
			       ($NumSources{$_} == 1) ? 'file' : 'files');
		&amp;PrintCounts('d', $str, '', \@Count_Names, $kind, $totals-&gt;{$_});
	    }
	}
    }
    for $kind (@kindList) {
	$str = sprintf("%s  (%d %s)",
		       "***** TOTAL$KIND{$kind} *****",
		       $NumSources,
		       ($NumSources == 1) ? 'file' : 'files');
	&amp;PrintCounts('d', $str, '', \@Count_Names, $kind, $totals)
	    if ($SECS{'totals'} &amp;&amp; ((!$SECS{'langtotals'}) || (@languages &gt; 1)));
    }
}

use Getopt::Long;

## main -- procedural interface to the script.
sub main {
    local @ARGV = @_;
    local $_;

    ## Default options
    $OPTS{'debug'}        = 0;
    $OPTS{'help'}         = 0;
    $OPTS{'delim_ignore'} = 0;
    $OPTS{'diff'}         = 0;
    $OPTS{'filelist'}     = 0;
    $OPTS{'ignore'}       = 0;
    $OPTS{'pred'}         = 0;
    $OPTS{'recurse'}      = 0;
    $OPTS{'counts'}       = "lines+blank+cmnts+ncsl+tptoks";
    $OPTS{'sections'}     = "header+files+langtotals+totals";
    $OPTS{'fromversion'}  = "";
    $OPTS{'prefix'}       = "";
    $OPTS{'toversion'}    = "";
    $OPTS{'vcstring'}     = "";
    $OPTS{'language'}     = [];
    $OPTS{'name'}         = [];
    $OPTS{'except'}       = [];
    $OPTS{'separate'}     = [];

    ## Parse options
    my $rc = &amp;GetOptions( \%OPTS,
			  "debug",
			  "help",
			  "output=s",
			  "counts=s",
			  "delim_ignore|delim-ignore",
			  "diff",
			  "except=s@",
			  "filelist",
			  "fromversion=s",
			  "ignore",
			  "language=s@",
			  "name=s@",
			  "pred",
			  "prefix=s",
			  "recurse",
			  "sections=s",
			  "separate=s@",
			  "toversion=s",
			  "vcstring=s"
			  );
    &amp;Usage(1) if ($OPTS{'help'});
    &amp;Usage(2) unless ($rc);
    &amp;Usage(2) if ((@ARGV == 0) &amp;&amp; (-t STDIN));

    ## Check for syntax errors
    if ($OPTS{'pred'} &amp;&amp; $OPTS{'fromversion'}) {
	&amp;ErrorMsg("The '-pred' and '-fromversion' options are incompatible!");
	++$ERRORS;
    }
    &amp;CheckLanguage(@{ $OPTS{'language'} }) if (@{ $OPTS{'language'} } &gt; 0);
    &amp;CheckSections($OPTS{'sections'}) if ($OPTS{'sections'});
    &amp;CheckCounts($OPTS{'counts'}) if ($OPTS{'counts'});

    &amp;Usage(2) if ($ERRORS);

    ## Redirect STDOUT if requested
    if ($OPTS{'output'}) {
	close STDOUT;
	open(STDOUT, "&gt;$OPTS{'output'}")
	    or &amp;FatalMsg("Unable to redirect STDOUT to $OPTS{'output'}: $!");
    }

    if ($OPTS{'pred'} || $OPTS{'fromversion'} || $OPTS{'toversion'}) {
	## Set mandatory options for using cdiff
	$OPTS{'diff'}     = 1;
	$OPTS{'separate'} = ['-----'];
    }
    %KIND = (
	     '' , '',
	     '&lt;', ' DELETED',
	     '&gt;', ' INSERTED'
	     );

    if (! $OPTS{'filelist'}) {
	@ARGV = ('-') if (0 == @ARGV);
	&amp;CountFiles(@ARGV);
    }
    else {
	## have to read list of filenames from input
	while (&lt;&gt;) {
	    chomp;
	    &amp;CountFiles($_);
	}
    }
    &amp;PrintTotals(\%Totals) if (($SECS{'totals'} || $SECS{'langtotals'}) &amp;&amp;
			       ((! $SECS{'files'}) || ($NumSources &gt; 1)));
}

#---------------------------------------------------------------------
package main;

&amp;Sclc::main(@ARGV);		##  unless ($DO_NOT_EXECUTE)

