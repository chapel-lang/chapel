//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO.FormattedIO;

/* START_EXAMPLE */
writef("My favorite %s is %i\n", "number", 7);

var s:string = "My favorite %s is %i".format("number", 7);
writeln(s);

// prints:
// My favorite number is 7
// My favorite number is 7
/* STOP_EXAMPLE */
