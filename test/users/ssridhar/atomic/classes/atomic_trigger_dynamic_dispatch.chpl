var a: int = 2;

class C {
  proc x(i: int) {
    a += i;
  }
}

class D : C {
  proc x(i: int) {
    a -= i;
  }
}

proc main {
  var c = new C(); 
  var d = new D(); 
  atomic c.x(2);
  d.x(2);
  writeln("a = ", a);
}