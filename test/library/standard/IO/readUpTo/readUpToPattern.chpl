use IO;

writeln("string - ref:");
readComplexPattern(true, openreader("pattern.txt"), ".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");
writeln("\nstring - return:");
readComplexPattern(false, openreader("pattern.txt"), ".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");

writeln("\nbytes - ref:");
readComplexPattern(true, openreader("pattern.txt"), b".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");
writeln("\nbytes - return:");
readComplexPattern(false, openreader("pattern.txt"), b".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");

proc readComplexPattern(param refBuffer, r, pattern: ?t) where refBuffer == true {
  var s: t;
  // read to pattern
  writeln(r.readUpTo(s, pattern), " ", s);

  // read pattern itself
  r.readLiteral(pattern);

  // read to EOF
  writeln(r.readUpTo(s, pattern), " ", s.strip());

  // try to read at EOF
  writeln(r.readUpTo(s, pattern), " ", s);
}

proc readComplexPattern(param refBuffer, r, pattern: ?t) where refBuffer == false {
  // read to pattern
  writeln(r.readUpTo(pattern));

  // read pattern itself
  r.readLiteral(pattern);

  // read to EOF
  writeln(r.readUpTo(pattern).strip());

  // try to read at EOF
  try {
    writeln(r.readUpTo(pattern));
  } catch e {
    writeln(e);
  }
}
