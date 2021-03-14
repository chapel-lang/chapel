
use Reflection;

class C {
  var x = 10;
  var A : [1..4] int = 5;
  var s = "hello world";

  proc postinit() {
    writeln("C.postinit");
  }
}

var c = new borrowed C();
writeln("has initializer: ", canResolveMethod(c, "init"));
writeln("c = ", c);
