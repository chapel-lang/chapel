module M {
  enum color {red, green, blue};
}

module M2 {
  use M only;
  var c: M.color;

  writeln(c:string);

  var c2: M.color;

  c2 = c;
  writeln(c2);

  for c in M.color do
    writeln(c);

  proc main() {
  }
}
