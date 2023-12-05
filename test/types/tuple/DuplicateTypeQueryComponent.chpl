// OK! Triggers redefinition error.
proc foo(tup : (int(?w), int(?w))) { writeln(w); }

// Should also trigger redefinition error.
proc foo((x, y): (real(?w), real(?w))) {
  writeln(w);
}

proc main() {
  foo((0, 8));
  foo((0.0, 8.0));
}
