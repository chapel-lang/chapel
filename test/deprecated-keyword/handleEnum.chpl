deprecated "Enum e is deprecated, use enum f"
enum e { a, b, c };
enum f { a = 1, b = 2, c = 3};

proc main() {
  var a = e.a;  // Should trigger the warning
  writeln(e.b); // Should trigger the warning
  var c = e.c;  // Should trigger the warning
  writeln(c);   // Shouldn't trigger the warning

  {
    use e;      // Should trigger the warning

    writeln(a); // Shouldn't trigger the warning
    writeln(b); // Shouldn't trigger the warning
    writeln(c); // Shouldn't trigger the warning
  }

  var a2 = f.a;
  writeln(f.b);
  var c2 = f.c;
  writeln(c2);
}
