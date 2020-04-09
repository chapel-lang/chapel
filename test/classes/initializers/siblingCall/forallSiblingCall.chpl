class Foo {
  var x: int;

  proc init(a: [] int) {
    forall i in a {
      this.init(i);
    }
  }

  proc init(xVal: int) {
    x = xVal;
  }
}

var foo = new shared Foo([1, 2, 3, 4]);
writeln(foo);
