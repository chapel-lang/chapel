class Foo {
  var x: int;

  proc init(a: [] int) {
    coforall val in a do
      writeln(val);
    x = 10;
  }
}

var foo = new unmanaged Foo([1, 2, 3, 4]);
writeln(foo);
delete foo;
