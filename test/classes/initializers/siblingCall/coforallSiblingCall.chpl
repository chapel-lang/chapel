class Foo {
  var x: int;

  proc init(a: [] int) {
    coforall i in a {
      this.init(i);
    }
  }

  proc init(xVal: int) {
    x = xVal;
  }
}

var foo = new unmanaged Foo([1, 2, 3, 4]);
writeln(foo);
delete foo;
