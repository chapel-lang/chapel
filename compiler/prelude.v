// V prelude, describing the initial environment

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
type list(a) __name "list" = a * list | void;
type ref(a) __name "ref";

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

__init__ __name "init":0;

//   internal
__make_tuple(...) __name "make_tuple":0;
__make_list(...) __name "make_list":0;
__make_vector(...) __name "make_vector":0;
__make_set(...) __name "make_set":0;
__make_continuation(...) __name "make_continuation":0;
__primitive(...) __name "primitive":0;

//   public
reply(...) __name "reply":0;

// symbol builtins
#operator __name "operator";
#"*" __name "deref";

// overloadable primitive operators

operator(a:any, #".", b:symbol)		   : __primitive a #"." b;
operator(a:any, #"*.", b:symbol)	   : __primitive a #"." b;
operator(a:anynum, #"*", b:anynum)	   : __primitive a #"*" b;
operator(a:anynum, #"/", b:anynum)	   : __primitive a #"/" b;
operator(a:anynum, #"%", b:anynum)	   : __primitive a #"%" b;
operator(a:anynum, #"+", b:anynum)	   : __primitive a #"+" b;
operator(a:anynum, #"-", b:anynum)	   : __primitive a #"-" b;
operator(a:int, #"<<", b:int)		   : __primitive a #"<<" b;
operator(a:int, #">>", b:int)		   : __primitive a #">>" b;
operator(a:anynum, #"<", b:anynum)	   : __primitive a #"<" b;
operator(a:anynum, #"<=", b:anynum)	   : __primitive a #"<=" b;
operator(a:anynum, #">", b:anynum)	   : __primitive a #">" b;
operator(a:anynum, #">=", b:anynum)	   : __primitive a #">=" b;
operator(a:anynum, #"==", b:anynum)	   : __primitive a #"==" b;
operator(a:anynum, #"!=", b:anynum)	   : __primitive a #"!=" b;
operator(a:int, #"&", b:int)		   : __primitive a #"&" b;
operator(a:int, #"^", b:int)		   : __primitive a #"^" b;
operator(a:int, #"|", b:int)		   : __primitive a #"|" b;
operator(a:any, #"&&", b:any)		   : __primitive a #"&&" b;
operator(a:any, #"||", b:any)		   : __primitive a #"||" b;
operator(a:any, #"=", b:any)		   : __primitive a #"=" b;
operator(a:anynum, #"*=", b:anynum)	   : __primitive a #"*=" b;
operator(a:anynum, #"/=", b:anynum)	   : __primitive a #"/=" b;
operator(a:anynum, #"%=", b:anynum)	   : __primitive a #"%=" b;
operator(a:anynum, #"+=", b:anynum)	   : __primitive a #"+=" b;
operator(a:anynum, #"-=", b:anynum)	   : __primitive a #"-=" b;
operator(a:int, #"<<=", b:int)		   : __primitive a #"<<=" b;
operator(a:int, #">>=", b:int)		   : __primitive a #">>=" b;
operator(a:int, #"&=", b:int)		   : __primitive a #"&=" b;
operator(a:int, #"|=", b:int)		   : __primitive a #"|=" b;
operator(a:int, #"^=", b:int)		   : __primitive a #"^=" b;
operator(a:any, #"->", b:symbol)	   : __primitive (__primitive #"*" a) #"." b;
operator(a:any, #"->*", b:symbol)	   : __primitive (__primitive #"*" a) #"." b; 
operator(a:anynum, #"^^", b:anynum)	   : __primitive a #"^^" b;
operator(#"++", a:anynum)		   : __primitive #"++" a;
operator(#"--", a:anynum)		   : __primitive #"--" a;
operator(#"+", a:anynum)		   : __primitive #"+" a;
operator(#"-", a:anynum)		   : __primitive #"-" a;
operator(#"~", a:anynum)		   : __primitive #"~" a;
operator(a:anynum, #"|", b:anynum)	   : __primitive a #"|" b;
operator(#"(", a:symbol, b:any)		   : __primitive #"(" a b;
operator(a:anynum, #"++")		   : __primitive a #"++";
operator(a:anynum, #"--")		   : __primitive a #"--";

// iterators

type iterator(a);
type iteratable = {
  type element_type;
  elements : iterator(element_type);
};


// domains

type domain;
type decomposition;
type arithmetic_domain(rank:symbol, distribute:decomposition, to:domain) : domain;
type opaque_domain(distribute:decomposition, to:domain) : domain;
type index(a:domain);
type subdomain(a:domain) = {
  size : int;
  lbound : int -> int;
  ubound : int -> int;
};

type decomposition : catagory = {
  type source : domain, target : domain;
  locale : index(source) -> index(target);
};

type simple_block_decomposition : decomposition = {
  where source : arithmetic_domain(1), target : arithmetic_domain(1);
  s : source;
  t : target;
  chunk : int;
  mod : int;
};

simple_block_decomposition::local(i): {
  k : i / chunk;
  k : (i - s.lbound(1)) / chunk;
  if (k < mod) 
    k += 1;
  k + t.lbound(1)
};

type map(d: domain, a) : iteratable = {
  elements : iterator(a);
  indexes : iterator(index(d));
  operator: #"[" * index(d) -> a &;
};

operator(a:map, #"[", b:index(map::d)): 0;
