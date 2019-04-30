// Tests using a helper module with a require statement involving a .c file when
// both the helper module and the .c file live in a different directory than the
// file that relies on them (and is being compiled).
use helperMod;

writeln(callBar());
