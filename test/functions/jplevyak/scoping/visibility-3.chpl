fun j() { return 4; }
var a = => j();
writeln(a());

fun g() { 
  fun f() { return 1; }
  return a(); 
}

fun h() {
  fun f() { return 2; }
  a = => f();
  writeln(g());
}

fun i() {
  fun f() { return 3; }
  a = => f();
  writeln(g());
}

h();
i();
