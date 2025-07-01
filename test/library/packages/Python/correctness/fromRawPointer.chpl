// test of the internal interface, creating a Value from a pointer

use Python;
var interp = new Interpreter();

var g = new GIL();
var mod_ptr = interp.importModuleInternal("math");
g.release();
var mod = new Value(interp, mod_ptr);
writeln(mod.get("__name__"));
