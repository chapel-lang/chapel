//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Path;

/* START_EXAMPLE */
writeln(basename("/foo/bar/baz")); // Prints "baz"
writeln(basename("/foo/bar/")); // Prints "", because of the empty string
/* STOP_EXAMPLE */
