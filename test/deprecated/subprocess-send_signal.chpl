use Subprocess;
use Sys only SIGKILL;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.send_signal(Sys.SIGKILL);
while sub.running do
  sub.poll();
assert(sub.exitCode == -Sys.SIGKILL);
