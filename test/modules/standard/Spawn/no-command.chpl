use Spawn;

var sub = spawn(["./junk"]);

sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


