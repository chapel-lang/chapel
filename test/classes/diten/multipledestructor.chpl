class C {
  def ~C() { writeln("In ~C #1"); }
}
def C.~C() { writeln("In ~C #2"); }

def main {
  var c = new C();
  delete c;
}
