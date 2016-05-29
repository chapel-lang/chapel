require 'testit.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f);

proc baz(f) {
  f();
}

var x = c_FnPtrTo(foo);
var y = foo:

compilerWarning(x.type:string);
  
bar(x);
baz(y);
 