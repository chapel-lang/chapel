
class A {
 def hello() { writeln("hello from A!"); }
}

class N {
  type X;
  var x = new X(); 
}

def main() {
  var n = new N(X=A);
  n.x.hello();
}

