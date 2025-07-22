/*START_EXAMPLE*/
use Python;
var interp = new Interpreter();
var mod = interp.importModule("math");

// the following two lines are equivalent
var fac1: Value = mod.get("factorial");
var fac2: Value = new Function(mod, "factorial");
/*STOP_EXAMPLE*/
