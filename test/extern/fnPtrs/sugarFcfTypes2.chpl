require 'externCFns.h';

proc foo(x: int) {
  writeln("foo() got: ", x);
  return x+1;
}
extern proc barIntInt(f: c_fn_ptr);

proc baz(f) {
  writeln("Chapel callsite got: ", f(33));
}

barIntInt(foo);
