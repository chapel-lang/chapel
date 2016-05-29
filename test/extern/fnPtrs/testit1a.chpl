require 'testit.h';

proc foo() {
  writeln("Hi!");
}

extern proc bar(f);

proc baz(f) {
  f();
}

var x = c_fnPtrTo(foo);
var y = foo;
  
bar(x);
baz(y);
 