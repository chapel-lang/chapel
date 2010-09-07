config const n = 10;
config type myType = int;

_extern def sizeof(type sizeofType): int(64);

var a, b: [0..n-1] myType;

writeln(sizeof(int)==sizeof(a._value.eltType));
