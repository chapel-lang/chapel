
class A {
 proc hello() { writeln("hello from A!"); }
}

class N {
  type X;
  var x = new owned X(); 
}

proc main() {
  var n = new owned N(X=unmanaged A);
  n.x.hello();
}

