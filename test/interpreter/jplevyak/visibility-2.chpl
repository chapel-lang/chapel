
def g(c) { 
  def f() { return 1; }
  return c(); 
}

def h() {
  def f() { return 2; }
  writeln(g(=>f()));
}

h();
