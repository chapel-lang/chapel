use Spawn;
use Time;

var sub = spawn(["sleep", "10"]);
sleep(1);
sub.kill();
sub.poll();
assert(sub.running == false);
assert(sub.exit_status != 0);
