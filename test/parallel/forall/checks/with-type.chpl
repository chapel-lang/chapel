// Ensure the compiler produces a graceful error.

type T = int;

forall Locales with (in T) {
  writeln();
}
