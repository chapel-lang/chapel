use IO;

config const IgnoreLeadingWS = false;

// test varying amount of ws in the literal
for param s in 0..<8 {
  writeln(stdin.matchLiteral(s*" " + "abc", ignoreWhitespace=true));
  stdin.matchLiteral("def\n");
}

// test multiple ws characters in the literal
writeln(stdin.matchLiteral("\n  a b c", ignoreWhitespace=true));
stdin.matchLiteral(" d e f");

// test a whitespace-only literal
writeln(stdin.matchLiteral(" \n", ignoreWhitespace=true));
