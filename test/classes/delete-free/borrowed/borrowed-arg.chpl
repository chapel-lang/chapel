proc f(arg:borrowed) {
  writeln("in f(borrowed)");
}

proc f(arg:unmanaged) {
  writeln("in f(unmanaged)");
}

proc f(arg) {
  writeln("in f(generic)");
}

class MyClass {
  var x:int;
}

var b = new borrowed MyClass(1);
var u = new unmanaged MyClass(1);
var i = 1;

writeln("f(b)");
f(b);
writeln("f(u)");
f(u);
writeln("f(i)");
f(i);

delete u;
