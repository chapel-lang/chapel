use Subprocess;

var sub = spawn(["ls", "test.txt"]);
sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


