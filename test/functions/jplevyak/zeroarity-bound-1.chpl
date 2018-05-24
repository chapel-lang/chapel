class C {
  proc f { return 1; }
  proc g() { return 2; }
  
}

var c = new unmanaged C();

writeln(c.f);
writeln(c.g());

// error: writeln(c.g);
// error: writeln(c.f());

delete c;
