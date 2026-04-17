/*START_EXAMPLE*/
use Python, IO;

var interp = new Interpreter();
var func = interp.compileLambda("lambda x,: print(x)");

writeln("Hello from Chapel");
writeln("Let's call some Python!");
IO.stdout.flush(); // flush the Chapel output buffer before calling Python

func("Hello, World!");
func("Goodbye, World!");
interp.flush(); // flush the Python output buffer before calling Chapel again

writeln("Back to Chapel");
/*STOP_EXAMPLE*/
