// ensure "cannot pass lvalue" errors are reported
use IO;

proc with_constref_arg(const ref x_constref) { writeln(x_constref); }
proc with_inout_arg(inout x_inout) { writeln(x_inout); }

const xxx = 3;
with_constref_arg(xxx); //OK
with_constref_arg(5); // currently an error
with_inout_arg(xxx);
with_inout_arg(5);

// intentionally check functions in internal modules
var iii:int;
xxx += 5;
xxx <=> iii;
iii <=> xxx;
var f = openmem();
var r = f.reader();
r.readIt(xxx);
