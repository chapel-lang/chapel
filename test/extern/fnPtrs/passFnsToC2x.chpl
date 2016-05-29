require 'externCFns.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f);

proc baz(f) {
  f();
}

bar(c_fnPtrTo(foo));
bar(c_fnPtrTo(foo));
