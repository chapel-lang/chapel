use Subprocess;

var sub = spawn(["ls", "test.txt"]);
sub.wait(buffer=false);
assert(sub.running == false);
assert(sub.exitCode == 0);


