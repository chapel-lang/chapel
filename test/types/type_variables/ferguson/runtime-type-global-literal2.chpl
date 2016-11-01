type arrType = [1..8] real;

proc main() {

  var a:arrType;

  // now a.domain should be {1..8}
  writeln(a.domain);
  writeln(a);
}
