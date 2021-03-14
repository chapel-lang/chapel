use CPtr;
require 'externCFns.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f);

proc baz(f) {
  f();
}

bar(c_ptrTo(foo));
baz(foo);
bar(c_ptrTo(foo));
baz(foo);
