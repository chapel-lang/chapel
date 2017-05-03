class Foo {
  var x;
  var y;

  proc init(xVal = 3) {
    x = xVal;
    y = xVal + 2;
    super.init();
  }
}

var foo: Foo(string);
foo = new Foo("blah");
writeln(foo.type: string);
writeln(foo);
