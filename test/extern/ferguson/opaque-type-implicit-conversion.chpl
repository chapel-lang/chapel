extern type myint;
extern const global_myint: myint;

// this should enable explicit conversion (cast)
// not implicit conversion
inline operator :(x:myint, type t:int) {
  return 1;
}

proc foo(): int {
  //return "bla";      // error: can't initialize return value
  return global_myint; // expect a similar error
                       // (compiler shouldn't assume opaque type
                       //  can be implicitly converted to int)
}

writeln(foo());
