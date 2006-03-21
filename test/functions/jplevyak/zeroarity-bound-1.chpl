class C {
  fun f { return 1; }
  fun g() { return 2; }
  
}

var c = C();

writeln(c.f);
writeln(c.g());

// error: writeln(c.g);
// error: writeln(c.f());

