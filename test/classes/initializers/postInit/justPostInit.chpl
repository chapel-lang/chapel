
use Reflection;

class C {
  var x = 10;
  var A : [1..4] int = 5;
  var s = "hello world";

  proc postinit() {
    writeln("C.postinit");
  }
}

var ownC = new owned C();
var c = ownC.borrow();
writeln("has initializer: ", canResolveMethod(c, "init"));
writeln("c = ", c);
