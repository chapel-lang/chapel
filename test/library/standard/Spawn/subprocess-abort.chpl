use Subprocess;
use OS.POSIX only SIGABRT;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.abort();
while sub.running do
  sub.poll();
assert(sub.exitCode == -OS.POSIX.SIGABRT);
