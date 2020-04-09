// This test tracks trying to get the type of a generic var field from a fully
// instantiated type variable.  I'm using arrays here because that was the case
// where I wanted this functionality, but it applies to other generic types
use BlockDist;
const D = {1..5} dmapped Block({1..5});
type t = [D] int;
writeln(t: string);
// The following two lines work as expected.  The uncommented line should work
// as well, but does not
//var x: t;
//writeln(x._instance.type: string);
writeln(t._instance.type: string);
