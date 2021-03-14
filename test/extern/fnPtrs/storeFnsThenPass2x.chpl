use CPtr;
require 'externCFns.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f);

proc baz(f) {
  f();
}

var x = c_ptrTo(foo);
var y = foo;
var z = c_ptrTo(foo);
var w = foo;
  
bar(x);
baz(y);
bar(z);
baz(w);
