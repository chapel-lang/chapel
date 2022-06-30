use Subprocess;
use OS.POSIX only SIGTERM;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.terminate();
while sub.running do
  sub.poll();
assert(sub.exitCode == -OS.POSIX.SIGTERM);
