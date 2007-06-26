" Vim syntax file
" Language:	Chapel
" Maintainer:	
" Last Change:	2007 June 25

" Much was borrowed from the C and C++ syntax files.  Many thanks to those who
" contributed to them.

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" Read the C syntax to start with
if version < 600
  so <sfile>:p:h/c.vim
else
  runtime! syntax/c.vim
  unlet b:current_syntax
endif

" Chapel extentions
syn keyword chplStatement	this these use
"syn keyword chplAccess		public protected private
syn keyword chplType		const type param config domain index imag complex uint atomic real var inline bool
"syn keyword chplExceptions	throw try catch
"syn keyword chplOperator		operator typeid
"syn keyword chplOperator		and bitor or xor compl bitand and_eq or_eq xor_eq not not_eq
"syn match chplCast		"\<\(const\|static\|dynamic\|reinterpret\)_cast\s*<"me=e-1
"syn match chplCast		"\<\(const\|static\|dynamic\|reinterpret\)_cast\s*$"
"syn keyword chplStorageClass	mutable
syn keyword chplStructure	def class record cobegin begin
"syn keyword chplNumber		NPOS
syn keyword chplBoolean		true false
syn keyword chplConditional	then
syn keyword chplConstant	nil
syn keyword chplRepeat		coforall forall in

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
  HiLink chplAccess		chplStatement
  HiLink chplCast		chplStatement
  HiLink chplExceptions		Exception
  HiLink chplOperator		Operator
  HiLink chplStatement		Statement
  HiLink chplType		Type
  HiLink chplStorageClass	StorageClass
  HiLink chplStructure		Structure
  HiLink chplNumber		Number
  HiLink chplBoolean		Boolean
  HiLink chplConditional	Conditional
  HiLink chplConstant		Constant
  HiLink chplRepeat		Repeat
  delcommand HiLink
endif

let b:current_syntax = "chpl"

" vim: ts=8
