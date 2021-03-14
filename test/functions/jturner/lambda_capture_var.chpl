proc testme() {
  var i = 4;
  var f = lambda() { return i; };
  writeln(f());
}

testme();
