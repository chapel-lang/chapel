use Subprocess;
use Sys only SIGKILL;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.kill();
while sub.running do
  sub.poll();
assert(sub.exitCode == -Sys.SIGKILL);
