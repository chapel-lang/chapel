use IO;

// test varying amount of ws in the literal
for param s in 0..<8 {
  stdin.readLiteral(s*" " + "abc", ignoreWhitespace=true);
  stdin.readLiteral("def\n");
}

// test multiple ws characters in the literal
stdin.readLiteral("\n  a b c", ignoreWhitespace=true);
stdin.readLiteral(" d e f");

// test a whitespace-only literal
stdin.readLiteral(" ", ignoreWhitespace=true);
