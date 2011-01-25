proc j() { return 4; }
var a = => j();
writeln(a());

proc g() { 
  proc f() { return 1; }
  return a(); 
}

proc h() {
  proc f() { return 2; }
  a = => f();
  writeln(g());
}

proc i() {
  proc f() { return 3; }
  a = => f();
  writeln(g());
}

h();
i();
