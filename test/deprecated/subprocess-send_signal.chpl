use Subprocess;
use OS.POSIX;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.send_signal(SIGKILL);
while sub.running do
  sub.poll();
assert(sub.exitCode == -SIGKILL);
