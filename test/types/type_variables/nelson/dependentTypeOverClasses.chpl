
class A {
 proc hello() { writeln("hello from A!"); }
}

class N {
  type X;
  var x = new X(); 
}

proc main() {
  var n = new N(X=A);
  n.x.hello();
}

