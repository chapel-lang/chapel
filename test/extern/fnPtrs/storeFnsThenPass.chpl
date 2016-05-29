require 'externCFns.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f);

proc baz(f) {
  f();
}

var x = c_fnPtrTo(foo);
var y = foo;
  
bar(x);
baz(y);
 