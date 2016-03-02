use Spawn;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.terminate();
sub.poll();
assert(sub.running == false);
assert(sub.exit_status == -SIGTERM);
