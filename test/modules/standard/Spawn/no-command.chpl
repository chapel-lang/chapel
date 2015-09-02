use Spawn;

var sub = spawn(["./junk"]);

var error:syserr;
sub.wait(error=error);

assert(sub.running == false);

// Different systems report this kind of error
// at different times. On Mac OS X, error != 0,
// but on Linux, exit_status != 0.
assert(error != 0 || sub.exit_status != 0);


