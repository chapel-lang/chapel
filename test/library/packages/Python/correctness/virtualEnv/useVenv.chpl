// Verify that we haven't regressed on a bug where interpreter code would
// segfault with gasnet in a virtual environment
use Python;
use OS.POSIX;

// Ensures we'll actually exercise the code
writeln(getenv("VIRTUAL_ENV".c_str()) != nil);
// exercise the code
var interp = new Interpreter();
