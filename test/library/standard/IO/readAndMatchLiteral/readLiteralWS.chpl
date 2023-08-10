use IO;

for s in 0..<8 {
  stdin.readLiteral(2*" " + "abc", ignoreWhitespace=true);
  stdin.readLiteral("def\n");
}

stdin.readLiteral("\n  a b c", ignoreWhitespace=true);
stdin.readLiteral(" d e f");

stdin.readLiteral(" ", ignoreWhitespace=true);
