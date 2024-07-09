require "record-field-type-mismatch.h";

extern record foo {
  var x: int;
}

var myFoo: foo;
var i = myFoo.x;
// myFoo.x = 5;
