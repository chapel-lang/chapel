// https://github.com/chapel-lang/chapel/issues/17406

// The goal of this code is to trigger a compilation error
// in an internal module. Currently it uses + on (int,uint)
// and relies on operator + to invoke _throwOpError() in this case.
// If the implementation changes, another way to get a compilation error
// in an internal module may be needed.

var x: int, y: uint;

proc main() {
  bar();
}

proc bar() {
  foo();
}

proc foo() {
  writeln(x+y);
}
