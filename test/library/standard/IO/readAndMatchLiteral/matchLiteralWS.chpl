use IO;

for s in 0..<8 {
  writeln(stdin.matchLiteral(2*" " + "abc", ignoreWhitespace=true));
  stdin.matchLiteral("def\n");
}

writeln(stdin.matchLiteral("\n  a b c", ignoreWhitespace=true));
stdin.matchLiteral(" d e f");

writeln(stdin.matchLiteral(" ", ignoreWhitespace=true));
