record foo {
  var blarg: int;
  var bar: string;
};

type foo2 = foo;
var x: foo2;
var y: foo;

//This is OK:
y = new foo(1, "test");

writeln(y);

//but this is not:
x = new foo2(42, "Hello world"); // error: invalid use of 'new'

writeln(x);

