class Parent {
  proc f() { writeln("Parent f called");}
}

class Child : Parent {
  proc g() { writeln("Child g called");}
}

record wrapper {
  var c:unmanaged Child;
  forwarding c;
}

var c = new unmanaged Child();
var w : wrapper = new wrapper(c);
w.f();
w.g();

delete c;
