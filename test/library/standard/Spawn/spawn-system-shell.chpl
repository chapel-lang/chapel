use Subprocess;

var sub = spawnshell("echo moo");
sub.wait();
assert(sub.running == false);
assert(sub.exitCode == 0);


