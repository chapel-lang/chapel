use IO;

// test various amounts of leading whitespace in the literal
for param s in 0..<8 {
  writeln(stdin.matchLiteral(s*" " + "abc", ignoreWhitespace=false));
  stdin.matchLiteral("def\n");
}

// test multiple ws characters in the literal
writeln(stdin.matchLiteral("\n  a b c", ignoreWhitespace=false));
writeln(stdin.matchLiteral(" d e f", ignoreWhitespace=false));

// test a whitespace-only literal
writeln(stdin.matchLiteral("  \n", ignoreWhitespace=true));
