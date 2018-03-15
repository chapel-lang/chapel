
use Reflection;

class C {
  var x = 10;
  var A : [1..4] int = 5;
  var s = "hello world";

  proc postInit() {
    writeln("C.postInit");
  }
}

var c = new C();
writeln("has initializer: ", canResolveMethod(c, "init"));
writeln("c = ", c);
delete c;
