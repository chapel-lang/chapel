// Ensure the compiler produces a graceful error.

param T = "hi";

forall Locales with (in T) {
  writeln();
}
