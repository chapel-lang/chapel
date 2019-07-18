// Ensure the compiler produces a graceful error.

type T = int;

coforall Locales with (in T) {
  writeln();
}
