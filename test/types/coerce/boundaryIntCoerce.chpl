proc foo(x: real(32)) {
  writeln("In real32 foo");
}

proc foo(x: real(64)) {
  writeln("in real64 foo");
}

proc bar(x: complex(64)) {
  writeln("In complex64 bar");
}

proc bar(x: complex(128)) {
  writeln("In complex128 bar");
}

proc baz(x: real(32)) {
  writeln("In baz");
}

proc testit(type t) {
  param mx = max(t): t;
  param mn = min(t): t;

  writeln(t: string);
  foo(mx);
  foo(mn);
  bar(mx);
  bar(mn);
  if numBits(t) < 64 {
    baz(mx);
    baz(mn);
  }
  writeln();
}

testit(int(64));
testit(uint(64));
testit(int(32));
testit(uint(32));
testit(int(8));
testit(uint(8));

param int32minAsInt: int(64) = min(int(32));
param int32maxAsInt: int(64) = max(uint(32));
foo(int32maxAsInt);
foo(int32minAsInt);
bar(int32maxAsInt);
bar(int32minAsInt);
baz(int32maxAsInt);
baz(int32minAsInt);