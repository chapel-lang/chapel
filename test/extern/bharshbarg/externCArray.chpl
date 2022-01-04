use SysCTypes, CPtr;

require "helper.h";

config const n = 2;

proc main() {
  extern const foo : c_ptr(c_int);
  foo[n] = 42;
  for i in 0..4 {
    writeln(foo[i]);
  }
}
