// This program shows that one can create an enum
// that stores uints and that the automatic addition
// performed by the compiler will cause the enum
// to be stored in a uint rather than in an int that
// overflows.

// While the Enumerated Types section of the spec could
// certainly be clearer, it says that the enum constants
// are treated as parameters "of integral type" (ie not
// just 'int' but maybe 'uint) and that an ommitted
// initializer means it should be one higher than the
// previous constant.

// In the example, if the compiler does not decide to
// treat these enum constants as uint, 'b' would be
// less than 'a' rather than one higher.

enum myenum {
  a = 0x7fffffffffffffff,
  b,
  c = b + 2
}

proc writeint(x:int) {
  writef(" int %xi\n", x);
}
proc writeint(x:uint) {
  writef("uint %xu\n", x);
}

var t = myenum.a;
writeint(t);
writeint(myenum.b);
writeint(myenum.c);
