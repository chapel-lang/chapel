// This test shows that the Intel and PGI compilers incorrectly inline an
// expression that is intended to occupy only 32 bits.
// This inlining takes place only if optimization is enabled and the expression
// is used in another expression.  So only the second writeln below fails.
// In particular, the emitted code contains an incq without a following 32-bit
// sign extension.
// Assuming that the result of integer overflow is unspecified, then this
// behavior is OK, but Chapel should try to protect users from such
// platform-dependencies.

config const maxS = max(int(32));

proc main() {
  const maxS1 = maxS + 1;
  const m = 10;

  writeln(maxS % m);
  writeln((maxS+1) % m);
  writeln(maxS1 % 10);
}
