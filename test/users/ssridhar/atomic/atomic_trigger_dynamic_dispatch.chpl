class C {
  def x() {
    writeln("In C's x");
  }
}

class D : C {
  def x() {
    writeln("In D's x");
  }
}

def main {
  var c = new C(); 
  var d = new D(); 
  atomic c.x();
  d.x();
}