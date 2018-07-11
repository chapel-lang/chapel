record G {
  param x;
}

proc test(param name, type t, param p) {
  param q = p:t;
  var g = new G(q);
  writef("%-8s param type: %-8s g type: %s\n",
         name, t:string, g.type:string);
}

proc main() {
  test("int",      int,      1);
  test("int(8)",   int(8),   1);
  test("int(16)",  int(16),  1);
  test("int(32)",  int(32),  1);
  test("int(64)",  int(64),  1);

  test("uint",     uint,     1);
  test("uint(8)",  uint(8),  1);
  test("uint(16)", uint(16), 1);
  test("uint(32)", uint(32), 1);
  test("uint(64)", uint(64), 1);

  test("bool",     bool,     true);
  test("bool(8)",  bool(8),  true);
  test("bool(16)", bool(16), true);
  test("bool(32)", bool(32), true);
  test("bool(64)", bool(64), true);
}
