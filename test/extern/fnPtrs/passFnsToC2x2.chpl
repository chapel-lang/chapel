use CPtr;
require 'externCFns.h';

proc foo(x: int) {
  writeln("foo() got: ", x);
  return x+1;
}

extern proc barIntInt(f);

proc baz(f) {
  writeln("Chapel callsite got: ", f(33));
}

barIntInt(c_ptrTo(foo));
barIntInt(c_ptrTo(foo));
