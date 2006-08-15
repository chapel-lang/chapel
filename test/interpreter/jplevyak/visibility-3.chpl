def j() { return 4; }
var a = => j();
writeln(a());

def g() { 
  def f() { return 1; }
  return a(); 
}

def h() {
  def f() { return 2; }
  a = => f();
  writeln(g());
}

def i() {
  def f() { return 3; }
  a = => f();
  writeln(g());
}

h();
i();
