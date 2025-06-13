//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Path;

/* START_EXAMPLE */
writeln(joinPath("/foo/bar", "/baz")); // Prints "/baz"
writeln(joinPath("/foo", "./baz")); // Prints "/foo/./baz"
writeln(joinPath("/foo/", "", "./baz")); // Prints "/foo/./baz"
/* STOP_EXAMPLE */
