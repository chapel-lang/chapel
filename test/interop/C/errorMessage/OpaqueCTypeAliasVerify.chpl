// Should compile even with '--verify'.

pragma "opaque c type alias"
extern "char" type opaqueChar1;

pragma "opaque c type alias"
extern "char" type opaqueChar2;

pragma "opaque c type alias"
extern "int" type opaqueInt;

export proc foo(x: opaqueChar1, y: opaqueChar2, z: opaqueInt) {}
