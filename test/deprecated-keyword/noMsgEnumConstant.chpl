enum foo { deprecated a, b, c };

proc main() {
  var a1 = foo.a; // Should trigger the warning
  writeln(a1);    // Shouldn't trigger the warning
  var b1 = foo.b; // Shouldn't trigger the warning
  writeln(b1);    // Shouldn't trigger the warning
  var c1 = foo.c; // Shouldn't trigger the warning
  writeln(c1);    // Shouldn't trigger the warning

  {
    use foo;    // Shouldn't trigger the warning
    writeln(a); // Should trigger the warning
    writeln(b); // Shouldn't trigger the warning
    writeln(c); // Shouldn't trigger the warning
  }
}
