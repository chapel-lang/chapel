class Foo {
  var x: int;

  proc init(a: [] int) {
    forall i in a {
      writeln(i);
      super.init();
    }
  }
}

var foo = new owned Foo([1, 2, 3, 4]);
writeln(foo);
