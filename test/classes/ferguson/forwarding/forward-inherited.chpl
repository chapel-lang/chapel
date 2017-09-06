class Parent {
  proc f() { writeln("Parent f called");}
}

class Child : Parent {
  proc g() { writeln("Child g called");}
}

record wrapper {
  var c:Child;
  forwarding c;
}

var w : wrapper = new wrapper(new Child());
w.f();
w.g();
