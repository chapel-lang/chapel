
fun g(c) { 
  fun f() { return 1; }
  return c(); 
}

fun h() {
  fun f() { return 2; }
  writeln(g(=>f()));
}

h();
