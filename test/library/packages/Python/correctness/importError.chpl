// make sure that exceptions thrown from initializers are handled properly
use Python;
var interp = new Interpreter();

try {
  var m = new Module(interp, "mymod");
  writeln("module: ", m);
} catch e: ImportError {
  writeln("Caught ImportError: ", e);
} catch {
  writeln("Caught unknown exception");
}

try {
  var m = interp.importModule("mymod2");
  writeln("module: ", m);
} catch e: ImportError {
  writeln("Caught ImportError: ", e);
} catch {
  writeln("Caught unknown exception");
}
