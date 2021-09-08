use Spawn;

var sub = spawn(["ls", "test.txt"]);
sub.wait();
assert(sub.running == false);
assert(sub.exitCode == 0);


