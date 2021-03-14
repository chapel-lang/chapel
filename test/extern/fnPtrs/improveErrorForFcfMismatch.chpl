require 'testit.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f:c_fn_ptr);

proc baz(f) {
  f();
}

bar(foo);
