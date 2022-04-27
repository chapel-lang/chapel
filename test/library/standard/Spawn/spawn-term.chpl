use Subprocess;
use Sys only SIGTERM;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.terminate();
while sub.running do
  sub.poll();
assert(sub.exitCode == -Sys.SIGTERM);
