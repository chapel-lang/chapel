use Subprocess;
use OS.POSIX only SIGKILL;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.send_signal(OS.POSIX.SIGKILL);
while sub.running do
  sub.poll();
assert(sub.exitCode == -OS.POSIX.SIGKILL);
