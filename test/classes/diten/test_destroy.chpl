class C {
  var a, b, c: int;
}

def C.~C() { writeln("In destructor"); }


def main {
  var c: C = new C(1,2,3);
  delete c;
}
