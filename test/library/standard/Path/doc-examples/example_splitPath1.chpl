//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Path;

/* START_EXAMPLE */
writeln(splitPath("foo/bar")); // Prints "(foo, bar)"
writeln(splitPath("bar")); // Prints "(, bar)"
writeln(splitPath("foo/")); // Prints "(foo, )"
writeln(splitPath("")); // Prints "(, )"
writeln(splitPath("/")); // Prints "(/, )"
/* STOP_EXAMPLE */
