use CTypes;

pragma "opaque c type alias"
extern "char" type opaqueChar;

pragma "opaque c type alias"
extern "char" type anotherOpaqueChar;

pragma "opaque c type alias"
extern "int" type opaqueInt;

type argArray = c_ptr(c_ptr(opaqueChar));

export proc signature0(x: opaqueChar) {}
export proc signature1(x: c_ptr(opaqueChar)) {}
export proc signature2(x: argArray) {}
export proc signature3(x: anotherOpaqueChar) {}
export proc signature4(x: opaqueInt) {}
export proc signature5(argc: opaqueInt, argv: argArray): opaqueInt {
  return __primitive("cast", opaqueInt, 0);
}

export proc foo(x1: argArray) {
  const x2 = __primitive("cast", c_ptr(c_ptr(c_char)), x1);
  // In the body, can use 'foo' with 'x2'...
  writeln(x1.type:string);
  writeln(x2.type:string);
  var x3: int;
  writeln(x3.type:string);
}

var g1: opaqueChar;
var g2: c_ptr(opaqueChar);
var g3: argArray;

proc main() {
  var x1: opaqueChar;
  var x2: c_ptr(opaqueChar);
  var x3: argArray;
}
