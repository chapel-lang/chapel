// V Prelude: The Initial Environment

in system __name "system";

// simple builtin types

type anyclass __name "anyclass";
type any __name "any";
type module __name "module";
type symbol __name "symbol";
type function __name "function";
type continuation __name "continuation";
type vector __name "vector";
type tuple __name "tuple";
type void __name "void" : tuple;
type object __name "object" = { };
type list __name "list";
type ref __name "ref";
type value __name "value";

type catagory __name "catagory";
type set __name "set" : catagory;
type sequence __name "sequence";
type domain __name "domain";
type array __name "array";

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
type char __name "char" = uint8;
type size __name "size" = uint32;
type bool __name "bool" = int;
type string __name "string";
type enum_element __name "enum_element" = int;

// builtin data

null __name "null" : (); 

// global initialization

__init__ __name "init":0;

//   public builtin functions
reply: #reply __name "reply";

// meta functions
anyclass::class(a:anyclass)		   : #__meta_apply self a;

// overloadable primitive operators

__init(a:any)				   : a; 
new(a:anyclass) __name "new_object"	   : { o: #__new a; o.__init; o };
operator(a:any, #".", b:symbol)		   : #__operator a #"." b;
operator(a:any, #"*.", b:symbol)	   : #__operator a #"." b;
operator(a:anynum, #"*", b:anynum)	   : #__operator a #"*" b;
operator(a:anynum, #"/", b:anynum)	   : #__operator a #"/" b;
operator(a:anynum, #"%", b:anynum)	   : #__operator a #"%" b;
operator(a:anynum, #"+", b:anynum)	   : #__operator a #"+" b;
operator(a:anynum, #"-", b:anynum)	   : #__operator a #"-" b;
operator(a:int, #"<<", b:int)		   : #__operator a #"<<" b;
operator(a:int, #">>", b:int)		   : #__operator a #">>" b;
operator(a:anynum, #"<", b:anynum)	   : #__operator a #"<" b;
operator(a:anynum, #"<=", b:anynum)	   : #__operator a #"<=" b;
operator(a:anynum, #">", b:anynum)	   : #__operator a #">" b;
operator(a:anynum, #">=", b:anynum)	   : #__operator a #">=" b;
operator(a:anynum, #"==", b:anynum)	   : #__operator a #"==" b;
operator(a:anynum, #"!=", b:anynum)	   : #__operator a #"!=" b;
operator(a:int, #"&", b:int)		   : #__operator a #"&" b;
operator(a:int, #"^", b:int)		   : #__operator a #"^" b;
operator(a:int, #"|", b:int)		   : #__operator a #"|" b;
operator(a:any, #"&&", b:any)		   : #__operator a #"&&" b;
operator(a:any, #"||", b:any)		   : #__operator a #"||" b;
operator(a:ref, #"=", b:any)		   : #__operator a #"=" b;
operator(a:any, #"=", b:any)		   : b;
operator(a:anynum, #"*=", b:anynum)	   : #__operator a #"*" b;
operator(a:anynum, #"/=", b:anynum)	   : #__operator a #"/" b;
operator(a:anynum, #"%=", b:anynum)	   : #__operator a #"%" b;
operator(a:anynum, #"+=", b:anynum)	   : #__operator a #"+" b;
operator(a:anynum, #"-=", b:anynum)	   : #__operator a #"-" b;
operator(a:anynum, #"<<=", b:int)	   : #__operator a #"<<" b;
operator(a:anynum, #">>=", b:int)	   : #__operator a #">>" b;
operator(a:anynum, #"&=", b:int)	   : #__operator a #"&" b;
operator(a:anynum, #"|=", b:int)	   : #__operator a #"|" b;
operator(a:anynum, #"^=", b:int)	   : #__operator a #"^" b;
operator(a:any, #"->", b:symbol)	   : #__operator (#__operator #"*" a) #"." b;
operator(a:any, #"->*", b:symbol)	   : #__operator (#__operator #"*" a) #"." b; 
operator(a:anynum, #"^^", b:anynum)	   : #__operator a #"^^" b;
operator(#"++", a:anynum)		   : a + 1;
operator(#"--", a:anynum)		   : a - 1;
operator(#"+", a:anynum)		   : #__operator #"+" a;
operator(#"-", a:anynum)		   : #__operator #"-" a;
operator(#"~", a:anynum)		   : #__operator #"~" a;
operator(#"!", a:any)			   : #__operator #"!" a;
operator(#"*", a:any)			   : #__operator #"*" a;
operator(#"&", var a:any)		   : #__operator #"&" a;
operator(a:anynum, #"|", b:anynum)	   : #__operator a #"|" b;
operator(#"(", a:symbol, b:any)		   : #__operator #"(" a b;
operator(a:anynum, #"++")		   : a + 1;
operator(a:anynum, #"--")		   : a - 1;

// tuples indexing

tuple::self(a:anyint) : #__index self a;

// generic printing

print a : #__primitive #print a;
