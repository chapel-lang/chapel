use IO;

writeln("string - ref:");
readComplexPattern(true, openReader("pattern.txt"), ".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");
writeln("\nstring - return:");
readComplexPattern(false, openReader("pattern.txt"), ".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");

writeln("\nbytes - ref:");
readComplexPattern(true, openReader("pattern.txt"), b".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");
writeln("\nbytes - return:");
readComplexPattern(false, openReader("pattern.txt"), b".... . .-.. .-.. --- .-- --- .-. .-.. -.. -.-.--");

proc readComplexPattern(param refBuffer, r, pattern: ?t) where refBuffer == true {
  var s: t;
  // read to pattern
  writeln(r.readTo(pattern, s), " ", s);

  // read pattern itself
  r.readLiteral(pattern);

  // read to EOF
  writeln(r.readTo(pattern, s), " ", s.strip());

  // try reading at EOF (should return 'false')
  writeln(r.readTo(pattern, s), " ", s);
}

proc readComplexPattern(param refBuffer, r, pattern: ?t) where refBuffer == false {
  // read to pattern
  writeln(r.readTo(pattern));

  // read pattern itself
  r.readLiteral(pattern);

  // read to EOF
  writeln(r.readTo(pattern).strip());

  // try reading at EOF (should throw UnexpectedEofError)
  try {
    writeln(r.readTo(pattern));
  } catch e {
    writeln(e);
  }
}
