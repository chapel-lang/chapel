use Spawn;

var sub = spawnshell("echo moo");
sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


