proc testme() {
  var i = 4;
  var f = proc(): int { return i; };
  writeln(f());
}

testme();
