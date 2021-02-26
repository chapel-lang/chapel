" Vim syntax file
" Language:	Chapel
" Maintainer:	James Dinan <dinan@cray.com>
" Last Change:	2008 June 18
" License:      Vim License

" Much was borrowed from the C and C++ syntax files.  Many thanks to those who
" contributed to them.

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

""" Begin: stuff lifted from the C syntax file

syn keyword	cTodo		contained TODO FIXME XXX

" cCommentGroup allows adding matches for special things in comments
syn cluster	cCommentGroup	contains=cTodo

" String and Character constants
" Highlight special characters (those which have a backslash) differently
syn match	cSpecial	display contained "\\\(x\x\+\|\o\{1,3}\|.\|$\)"
if !exists("c_no_utf")
  syn match	cSpecial	display contained "\\\(u\x\{4}\|U\x\{8}\)"
endif
if exists("c_no_cformat")
  syn region	cString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,@Spell
  syn region	cString		start=+L\='+ skip=+\\\\\|\\'+ end=+'+ contains=cSpecial,@Spell
  syn region	cString		start=+L\=b"+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,@Spell
  syn region	cString		start=+L\=b'+ skip=+\\\\\|\\'+ end=+'+ contains=cSpecial,@Spell
  syn region	cString		start=+L\=c"+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,@Spell
  syn region	cString		start=+L\=c'+ skip=+\\\\\|\\'+ end=+'+ contains=cSpecial,@Spell
  " cCppString: same as cString, but ends at end of line
  syn region	cCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,@Spell
  syn region	cCppString	start=+L\='+ skip=+\\\\\|\\'\|\\$+ excludenl end=+'+ end='$' contains=cSpecial,@Spell
else
  syn match	cFormat		display "%\(\d\+\$\)\=[-+' #0*]*\(\d*\|\*\|\*\d\+\$\)\(\.\(\d*\|\*\|\*\d\+\$\)\)\=\([hjlLtz]\|ll\)\=\([bdiuoxXDOUfeEgGcCsSpn]\|\[\^\=.[^]]*\]\)" contained
  syn match	cFormat		display "%%" contained
  syn region	cString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,cFormat,@Spell
  syn region	cString		start=+L\='+ skip=+\\\\\|\\'+ end=+'+ contains=cSpecial,cFormat,@Spell
  syn region	cString		start=+L\=b"+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,cFormat,@Spell
  syn region	cString		start=+L\=b'+ skip=+\\\\\|\\'+ end=+'+ contains=cSpecial,cFormat,@Spell
  syn region	cString		start=+L\=c"+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,cFormat,@Spell
  syn region	cString		start=+L\=c'+ skip=+\\\\\|\\'+ end=+'+ contains=cSpecial,cFormat,@Spell
  " cCppString: same as cString, but ends at end of line
  syn region	cCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,cFormat,@Spell
  syn region	cCppString	start=+L\='+ skip=+\\\\\|\\'\|\\$+ excludenl end=+'+ end='$' contains=cSpecial,cFormat,@Spell
endif

"
"when wanted, highlight trailing white space
if exists("c_space_errors")
  if !exists("c_no_trail_space_error")
    syn match	cSpaceError	display excludenl "\s\+$"
  endif
  if !exists("c_no_tab_space_error")
    syn match	cSpaceError	display " \+\t"me=e-1
  endif
endif

"catch errors caused by wrong parenthesis and brackets
" also accept <% for {, %> for }, <: for [ and :> for ] (C99)
" But avoid matching <::.
syn cluster	cParenGroup	contains=cParenError,cIncluded,cSpecial,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cUserCont,cUserLabel,cBitField,cCommentSkip,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom
if exists("c_no_bracket_error")
  syn region	cParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cCppString,@Spell
  " cCppParen: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cParen,cString,@Spell
  syn match	cParenError	display ")"
  syn match	cErrInParen	display contained "[{}]\|<%\|%>"
else
  syn region	cParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cErrInBracket,cCppBracket,cCppString,@Spell
  " cCppParen: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cErrInBracket,cParen,cBracket,cString,@Spell
  syn match	cParenError	display "[\])]"
  syn match	cErrInParen	display contained "[\]]\|<%\|%>"
  syn region	cBracket	transparent start='\[\|<::\@!' end=']\|:>' contains=ALLBUT,@cParenGroup,cErrInParen,cCppParen,cCppBracket,cCppString,@Spell
  " cCppBracket: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppBracket	transparent start='\[\|<::\@!' skip='\\$' excludenl end=']\|:>' end='$' contained contains=ALLBUT,@cParenGroup,cErrInParen,cParen,cBracket,cString,@Spell
  syn match	cErrInBracket	display contained "[);{}]\|<%\|%>"
endif

"integer number, or floating point number without a dot and with "f".
syn case ignore
syn match	cNumbers	display transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctalError,cOctal
" Same, but without octal error (for comments)
syn match	cNumbersCom	display contained transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctal
" decimal number
syn match	cNumber		display contained "\d\+\>"
syn match	cNumber		display contained "\d\(_\|\d\)*\>"
" hex number
syn match	cNumber		display contained "0x\x\+\>"
syn match	cNumber		display contained "0x\x\(_\|\x\)*\>"
" octal number
syn match	cOctal		display contained "0o\o\+\>"
syn match	cOctal		display contained "0o\o\(_\|\o\)*\>"
" binary number
syn match	cNumber         display contained "0b[0-1]\+\>"
syn match	cNumber         display contained "0b[0-1]\(_\|[0-1]\)*\>"
"floating point number, with dot, optional exponent
syn match	cFloat		display contained "\d\(_\|\d\)*\.\d\(_\|\d\)*\(e[-+]\=\d\(_\|\d\)*\)\=\>"
"floating point number, starting with a dot, optional exponent
syn match	cFloat		display contained "\.\d[_\d]*\(e[-+]\=\d[_\d]*\)\=\>"
" Avoid highlighting '..'
syn match	cNone		display "\.\{2}"
"floating point number, without dot, with exponent
syn match	cFloat		display contained "\d\(_\|\d\)*e[-+]\=\d\(_\|\d\)*\>"
if !exists("c_no_c99")
  "hexadecimal floating point number, optional leading digits, with dot, with exponent
  syn match	cFloat		display contained "0x\(\x\(_\|\x\)*\)\=\.\x\(_\|\x\)*p[-+]\=\d\(_\|\d\)*\>"
"  syn match	cFloat		display contained "0x\x*\.\x\+p[-+]\=\d\+[fl]\=\>"
  "hexadecimal floating point number, with leading digits, optional dot, with exponent
  syn match	cFloat		display contained "0x\x\(_\|\x\)*\.\=p[-+]\=\d\(_\|\d\)*\>"
"  syn match	cFloat		display contained "0x\x\+\.\=p[-+]\=\d\+[fl]\=\>"
endif

" flag an octal number with wrong digits
syn match	cOctalError	display contained "0o\o*[89a-f]\x*"
" let binary errors tag along with octal
syn match	cOctalError	display contained "0b[01]*[2-9a-f]\x*"
syn case match

if exists("c_comment_strings")
  " A comment can contain cString, cCharacter and cNumber.
  " But a "*/" inside a cString in a cComment DOES end the comment!  So we
  " need to use a special type of cString: cCommentString, which also ends on
  " "*/", and sees a "*" at the start of the line as comment again.
  " Unfortunately this doesn't very well work for // type of comments :-(
  syntax match	cCommentSkip	contained "^\s*\*\($\|\s\+\)"
  syntax region cCommentString	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end=+\*/+me=s-1 contains=cSpecial,cCommentSkip
  syntax region cComment2String	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end="$" contains=cSpecial
  syntax region  cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cComment2String,cCharacter,cNumbersCom,cSpaceError,@Spell
  syntax region cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell,cComment
else
  syn region	cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cSpaceError,@Spell
  syn region	cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cSpaceError,@Spell,cComment
endif
" keep a // comment separately, it terminates a preproc. conditional
syntax match	cCommentError	display "\*/"
syntax match	cCommentStartError display "/\*"me=e-1 contained

" Accept %: for # (C99)
"syn region	cPreCondit	start="^\s*\(%:\|#\)\s*\(if\|ifdef\|ifndef\|elif\)\>" skip="\\$" end="$" end="//"me=s-1 contains=cComment,cCppString,cCharacter,cCppParen,cParenError,cNumbers,cCommentError,cSpaceError
"syn match	cPreCondit	display "^\s*\(%:\|#\)\s*\(else\|endif\)\>"
"if !exists("c_no_if0")
"  syn region	cCppOut		start="^\s*\(%:\|#\)\s*if\s\+0\+\>" end=".\@=\|$" contains=cCppOut2
"  syn region	cCppOut2	contained start="0" end="^\s*\(%:\|#\)\s*\(endif\>\|else\>\|elif\>\)" contains=cSpaceError,cCppSkip
"  syn region	cCppSkip	contained start="^\s*\(%:\|#\)\s*\(if\>\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" contains=cSpaceError,cCppSkip
"endif
"syn region	cIncluded	display contained start=+"+ skip=+\\\\\|\\"+ end=+"+
"syn match	cIncluded	display contained "<[^>]*>"
"syn match	cInclude	display "^\s*\(%:\|#\)\s*include\>\s*["<]" contains=cIncluded
"syn match cLineSkip	"\\$"
"syn cluster	cPreProcGroup	contains=cPreCondit,cIncluded,cInclude,cDefine,cErrInParen,cErrInBracket,cUserLabel,cSpecial,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom,cString,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cParen,cBracket,cMulti
"syn region	cDefine		start="^\s*\(%:\|#\)\s*\(define\|undef\)\>" skip="\\$" end="$" end="//"me=s-1 contains=ALLBUT,@cPreProcGroup,@Spell
"syn region	cPreProc	start="^\s*\(%:\|#\)\s*\(pragma\>\|line\>\|warning\>\|warn\>\|error\>\)" skip="\\$" end="$" keepend contains=ALLBUT,@cPreProcGroup,@Spell

" Highlight User Labels
"syn cluster	cMultiGroup	contains=cIncluded,cSpecial,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cUserCont,cUserLabel,cBitField,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom,cCppParen,cCppBracket,cCppString
" Disabled due to generics conflict in chapel
"syn region	cMulti		transparent start='?' skip='::' end=':' contains=ALLBUT,@cMultiGroup,@Spell
" Avoid matching foo::bar() in C++ by requiring that the next char is not ':'
"syn cluster	cLabelGroup	contains=cUserLabel
"syn match	cUserCont	display "^\s*\I\i*\s*:$" contains=@cLabelGroup
"syn match	cUserCont	display ";\s*\I\i*\s*:$" contains=@cLabelGroup
"syn match	cUserCont	display "^\s*\I\i*\s*:[^:]"me=e-1 contains=@cLabelGroup
"syn match	cUserCont	display ";\s*\I\i*\s*:[^:]"me=e-1 contains=@cLabelGroup

"syn match	cUserLabel	display "\I\i*" contained

" Avoid recognizing most bitfields as labels
syn match	cBitField	display "^\s*\I\i*\s*:\s*[1-9]"me=e-1
syn match	cBitField	display ";\s*\I\i*\s*:\s*[1-9]"me=e-1

if exists("c_minlines")
  let b:c_minlines = c_minlines
else
  if !exists("c_no_if0")
    let b:c_minlines = 50	" #if 0 constructs can be long
  else
    let b:c_minlines = 15	" mostly for () constructs
  endif
endif
exec "syn sync ccomment cComment minlines=" . b:c_minlines

" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_c_syn_inits")
  if version < 508
    let did_c_syn_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif

  HiLink cFormat		cSpecial
  HiLink cCppString		cString
  HiLink cCommentL		cComment
  HiLink cCommentStart		cComment
"  HiLink cLabel			Label
"  HiLink cUserLabel		Label
"  HiLink cConditional		Conditional
"  HiLink cRepeat		Repeat
  HiLink cCharacter		Character
  HiLink cSpecialCharacter	cSpecial
  HiLink cNumber		Number
  HiLink cOctal			Number
  HiLink cOctalZero		PreProc	 " link this to Error if you want
  HiLink cFloat			Float
  HiLink cOctalError		cError
  HiLink cParenError		cError
  HiLink cErrInParen		cError
" Hack: avoid range errors of the form [1..M) .. allows other bracket errors 
"  HiLink cErrInBracket		cError
  HiLink cCommentError		cError
  HiLink cCommentStartError	cError
  HiLink cSpaceError		cError
  HiLink cSpecialError		cError
"  HiLink cOperator		Operator
"  HiLink cStructure		Structure
"  HiLink cStorageClass		StorageClass
"  HiLink cInclude		Include
"  HiLink cPreProc		PreProc
"  HiLink cDefine		Macro
"  HiLink cIncluded		cString
  HiLink cError			Error
  HiLink cStatement		Statement
"  HiLink cPreCondit		PreCondit
"  HiLink cType			Type
  HiLink cConstant		Constant
  HiLink cCommentString		cString
  HiLink cComment2String	cString
  HiLink cCommentSkip		cComment
  HiLink cString		String
  HiLink cComment		Comment
  HiLink cSpecial		SpecialChar
  HiLink cTodo			Todo
"  HiLink cCppSkip		cCppOut
"  HiLink cCppOut2		cCppOut
"  HiLink cCppOut		Comment

  delcommand HiLink
endif

""" End C syntax thievery

" FIXME:
"  - Do something with 'high, low, stride'?
"  - vars are sometimes labels when they start on a new line
"  - Better handing for ?
"  - Ranges need better support eg: [.., ..)

" Chapel extentions
syn keyword chplStatement	break return continue compilerWarning delete
syn keyword chplStatement	new delete this these use except only require import
syn keyword chplStatement	noinit init
syn keyword chplStatement	as module yield compilerError zip
syn keyword chplIntent		param type in out inout ref
syn keyword chplStorageClass    const config export extern var
syn keyword chplType            domain sparse subdomain range index imag complex int uint real bool
syn keyword chplType            file bytes string opaque integral numeric enumerated
syn keyword chplType            locale sync atomic single dmapped
syn keyword chplType            owned shared borrowed unmanaged
syn keyword chplType            nothing void
syn keyword chplOperator	on reduce scan by align
syn keyword chplStructure	class record union enum
syn keyword chplStructure	proc iter cobegin begin local sync let select where
syn keyword chplStructure	pragma inline with private public forwarding
syn keyword chplStructure	prototype override lifetime
syn keyword chplBoolean		true false
syn keyword chplConditional	if then else
syn keyword chplConstant	nil none
syn keyword chplRepeat		while for do coforall forall in serial
syn keyword chplLabel	        when otherwise label
syn keyword chplErrorHandling   throw throws try catch

" Folding
syn region scopeFold start="{" end="}" fold transparent

" The minimum and maximum operators in GNU C++
syn match chplMinMax "[<>]?"

" Default highlighting
if version >= 508 || !exists("did_chpl_syntax_inits")
  if version < 508
    let did_chpl_syntax_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif
  HiLink chplCast		chplStatement
  HiLink chplErrorHandling	chplStatement
  HiLink chplOperator		Operator
  HiLink chplStatement		Statement
  HiLink chplIntent		StorageClass
  HiLink chplStorageClass	StorageClass
  HiLink chplType		Type
  HiLink chplStructure		Structure
  HiLink chplNumber		Number
  HiLink chplBoolean		Boolean
  HiLink chplConditional	Conditional
  HiLink chplConstant		Constant
  HiLink chplRepeat		Repeat
  HiLink chplLabel		Label
  delcommand HiLink
endif

let b:current_syntax = "chpl"
