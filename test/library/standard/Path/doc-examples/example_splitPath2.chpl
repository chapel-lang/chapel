//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Path;

/* START_EXAMPLE */
var res = splitPath("foo/bar");
var dirnameVar = res(0);
var basenameVar = res(1);
writeln(dirnameVar + "/" + basenameVar); // Prints "foo/bar"
writeln(joinPath(dirnameVar, basenameVar)); // Prints "foo/bar"
/* STOP_EXAMPLE */
