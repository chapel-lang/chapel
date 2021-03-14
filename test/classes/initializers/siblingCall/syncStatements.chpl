// Verifies the behavior of initializers with sync statements around this.init()
// calls.  As with field initialization, there should be no issue with a sync
// statement around a this.init() call, as the sync statement itself introduces
// no parallelism or loop structures.
class Foo {
  var x = 11;

  proc init() {
    sync {
      this.init(-4);
    }
    writeln("whee!");
  }

  proc init(xVal) {
    x = xVal;
  }
}

var foo = new Foo();
writeln(foo);
delete foo;
