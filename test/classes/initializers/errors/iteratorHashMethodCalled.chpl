class IteratorHash : hashable {
  var x: uint;

  iter hash(): uint {
    return x.hash(); // This also triggers an error
  }
}

proc main() {
  use Map;

  var m = new map(IteratorHash, int);

  var i = new IteratorHash();
  m[i] = 3;
  writeln(m);
}
