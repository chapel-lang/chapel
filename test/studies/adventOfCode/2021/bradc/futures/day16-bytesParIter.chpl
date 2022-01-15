var b = b"abcdefghij";

forall c in b {
  writeln(b);
}

forall (i,c) in zip(0..<b.size, b) {
  writeln("character ", i, " of ", b, " is ", c);
}

forall (c,i) in zip(b, 0..) {
  writeln("character ", i, " of ", b, " is ", c);
}

