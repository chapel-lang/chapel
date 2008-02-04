class C {
  def f { return 1; }
  def g() { return 2; }
  
}

var c = new C();

writeln(c.f);
writeln(c.g());

// error: writeln(c.g);
// error: writeln(c.f());

