// Chapel Prelude

in system __name "system";

// simple builtin types

type symbol __name "symbol";
type function __name "function";
type module __name "module";
type continuation __name "continuation";
type any __name "any";
type vector __name "vector";
type tuple __name "tuple";
type void __name "void" : tuple;
type object __name "object" = { };
type list(a) __name "list";
type ref __name "ref";

type catagory __name "catagory";
type set __name "set" : catagory;

type int8 __name "int8";
type int16 __name "int16";
type int32 __name "int32";
type int64 __name "int64";
type int __name "int" = int32;
type uint8 __name "uint8";
type uint16 __name "uint16";
type uint32 __name "uint32";
type uint64 __name "uint64";
type uint __name "uint" = uint32;
type anyint __name "anyint" = 
  int8 | int16 | int32 | int64 |
  uint8 | uint16 | uint32 | uint64;

type float32 __name "float32";
type float64 __name "float64";
type float80 __name "float80";
type float128 __name "float128";
type float __name "float" = float64;
type anyfloat __name "anyfloat" = 
  float32 | float64 | float80 | float128;

type anynum __name "anynum" = anyint | anyfloat;
type size __name "size" = uint32;
type bool __name "bool" = int;
type enum_element __name "enum_element" = int;

// builtin data

null __name "null" : (); 

// global initialization

__init__ __name "init" : 0;

//   internal
function __make_tuple(...) __name "make_tuple" { 0 };
function __make_list(...) __name "make_list" { 0 } ;
function __make_vector(...) __name "make_vector" { 0 } ;
function __make_set(...) __name "make_set" { 0 } ;
function __make_continuation(...) __name "make_continuation" { 0 };
function __primitive(...) __name "primitive" { 0 } ;
function __new(...) __name "new" { 0 };

//   public
function reply(...) __name "reply" { 0 };

// symbol builtins
#operator __name "operator";
#"*" __name "deref";
#"&" __name "doref";

// overloadable primitive operators
function new(a:any) { __new a; }
function operator(a:any, #".", b:symbol)	{ __primitive a #"." b }
function operator(a:any, #"*.", b:symbol)	{ __primitive a #"." b }
function operator(a:anynum, #"*", b:anynum)	{ __primitive a #"*" b }
function operator(a:anynum, #"/", b:anynum)	{ __primitive a #"/" b }
function operator(a:anynum, #"%", b:anynum)	{ __primitive a #"%" b }
function operator(a:anynum, #"+", b:anynum)	{ __primitive a #"+" b }
function operator(a:anynum, #"-", b:anynum)	{ __primitive a #"-" b }
function operator(a:int, #"<<", b:int)		{ __primitive a #"<<" b }
function operator(a:int, #">>", b:int)		{ __primitive a #">>" b }
function operator(a:anynum, #"<", b:anynum)	{ __primitive a #"<" b }
function operator(a:anynum, #"<=", b:anynum)	{ __primitive a #"<=" b }
function operator(a:anynum, #">", b:anynum)	{ __primitive a #">" b }
function operator(a:anynum, #">=", b:anynum)	{ __primitive a #">=" b }
function operator(a:anynum, #"==", b:anynum)	{ __primitive a #"==" b }
function operator(a:anynum, #"!=", b:anynum)	{ __primitive a #"!=" b }
function operator(a:int, #"&", b:int)		{ __primitive a #"&" b }
function operator(a:int, #"^", b:int)		{ __primitive a #"^" b }
function operator(a:int, #"|", b:int)		{ __primitive a #"|" b }
function operator(a:any, #"&&", b:any)		{ __primitive a #"&&" b }
function operator(a:any, #"||", b:any)		{ __primitive a #"||" b }
function operator(a:ref, #"=", b:any)		{
  __primitive a #"=" b;
}
function operator(a:ref, #"*=", b:anynum)	{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"*" b)
}
function operator(a:ref, #"/=", b:anynum)	{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"/" b)
}
function operator(a:ref, #"%=", b:anynum)	{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"%" b)
}
function operator(a:ref, #"+=", b:anynum)	{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"+" b)
}
function operator(a:ref, #"-=", b:anynum)	{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"-" b)
}
function operator(a:ref, #"<<=", b:int)		{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"<<" b)
}
function operator(a:ref, #">>=", b:int)		{
  __primitive a #"=" (__primitive (__primitive #"*" a) #">>" b)
}
function operator(a:ref, #"&=", b:int)		{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"&" b)
}
function operator(a:ref, #"|=", b:int)		{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"|" b)
}
function operator(a:ref, #"^=", b:int)		{
  __primitive a #"=" (__primitive (__primitive #"*" a) #"^" b)
}
function operator(a:int, #"..", b:int)		{ sequence(a, b) }
function operator(a:any, #"->", b:symbol)	{ __primitive (__primitive #"*" a) #"." b }
function operator(a:any, #"->*", b:symbol)	{ __primitive (__primitive #"*" a) #"." b; }
function operator(a:anynum, #"^^", b:anynum)	{ __primitive a #"^^" b }
function operator(#"++", a:ref)			{
   __primitive a #"=" ((__primitive#"*" a) #"+" 1)
}
function operator(#"--", a:ref)			{ 
   __primitive a #"=" ((__primitive#"*" a) #"-" 1)
}
function operator(#"+", a:anynum)		{ __primitive #"+" a }
function operator(#"-", a:anynum)		{ __primitive #"-" a }
function operator(#"~", a:anynum)		{ __primitive #"~" a }
function operator(#"!", a:any)			{ __primitive #"!" a }
function operator(#"*", a:any)			{ __primitive #"*" a }
function operator(#"&", a:any)			{ __primitive #"&" a }
function operator(a:anynum, #"|", b:anynum)	{ __primitive a #"|" b }
function operator(#"(", a:symbol, b:any)	{ __primitive #"(" a b }
function operator(a:ref, #"++")			{ 
   __primitive a #"=" ((__primitive#"*" a) #"+" 1)
}
function operator(a:ref, #"--")			{ 
   __primitive a #"=" ((__primitive#"*" a) #"-" 1)
}


// domains

type domain;
type sequence __name "sequence";

class distribution {
  type Source_domain : domain;
  type Destination_domain : domain;
  var source : Source_domain;
  var destination : Destination_domain;
  function local(i : sequence) : sequence;
  function offset(i : sequence) : sequence;
}

class cyclic implements distribution {
  var width : int;
}

class domain(rank, distribute, target) {
  const rank : int;
  const index : sequence;
  type target : domain;
  type distribute: distribution;
}

function domain::class(i : int) {
}

function domain::class(s1 : sequence) {
  var d = new domain;
  d.rank = 1;
  d.index = s1;
  return d;
}

function domain::class(s1 : sequence, s2 : sequence) {
  var d = new domain;
  d.rank = 2;
  d.index = new sequence;
  d.index.first = s1.first * s2.first;
  d.index.last = s1.last * s2.last;
  d.index.step = s1.step * s2.step;
  return d;
}

class sequence {
  const first : tuple;
  const last : tuple;
  const step : tuple;
}

function sequence::class(f : int, l : int) {
  var s = new sequence;
  s.first = f;
  s.last = l;
  s.step = 1;
  return s;
}

function operator(a:sequence, #"*", b:sequence) {
  new sequence
}

