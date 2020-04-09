config const n = 10;
config type myType = int(32);

extern proc sizeof(type sizeofType): int(64);

var a, b: [0..n-1] myType;

writeln(sizeof(int(32))==sizeof(a.eltType));
