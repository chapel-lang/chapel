class Foo {
  var n: int;
  var dom = {1..n};
  var arr: [dom] int = n;
}

var foo = new Foo(5);
writeln(foo);
delete foo;
