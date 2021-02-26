use CPtr;
config param cPtrToChapel: bool = true;

require 'testit.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f:c_fn_ptr);

proc baz(f) {
  f();
}

if cPtrToChapel then
  baz(c_ptrTo(foo));
else
  bar(foo);
