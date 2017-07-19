class Foo {
  var x: int;

  proc init(a: [] int) {
    coforall i in a {
      writeln(i);
      super.init();
    }
  }
}

var foo = new Foo([1, 2, 3, 4]);
writeln(foo);
delete foo;
