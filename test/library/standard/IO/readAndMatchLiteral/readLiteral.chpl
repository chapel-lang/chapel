use IO;

// test various amounts of leading whitespace in the literal
for param s in 0..<8 {
  stdin.readLiteral(s*" " + "abc", ignoreWhitespace=false);
  stdin.readLiteral("def\n", ignoreWhitespace=false);
}

// test multiple ws characters in the literal
stdin.readLiteral("\n  a b c", ignoreWhitespace=false);
stdin.readLiteral(" d e f", ignoreWhitespace=false);

// test a whitespace-only literal
stdin.readLiteral("  \n", ignoreWhitespace=true);
