
proc g(c) { 
  proc f() { return 1; }
  return c(); 
}

proc h() {
  proc f() { return 2; }
  writeln(g(=>f()));
}

h();
