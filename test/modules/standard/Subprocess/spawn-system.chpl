use Subprocess;

var sub = spawn(["ls", "spawn1.chpl"]);
sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


