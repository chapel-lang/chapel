// Ensure the compiler produces a graceful error.

param T = "hi";

coforall Locales with (in T) {
  writeln();
}
