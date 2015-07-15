use definesPrivate;

writeln(bar);
// Ensures that the private module level variable foo is not visible
// when definesPrivate is used.

// Compopts #2 of this program ensures that setting foo with a compilation
// flag does not break the compilation of this program.
