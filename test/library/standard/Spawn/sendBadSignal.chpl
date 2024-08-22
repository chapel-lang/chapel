use Subprocess;
use CTypes;

extern proc chpl_SIGRTMAX(): c_int;

// Make sure the subprocess stays around long enough for us to potentially
// interact with it
var subproc = spawn(["sleep", "60"]);

try {
  subproc.sendPosixSignal(chpl_SIGRTMAX() + 1);
} catch e: IllegalArgumentError {
  // This is the intended behavior
  writeln("Caught IllegalArgumentError for too large signal");
} catch {
  // Catchall in case something else happened
  writeln("oops, didn't catch too large signal");
}

// Ensure the subprocess completes
subproc.wait();
