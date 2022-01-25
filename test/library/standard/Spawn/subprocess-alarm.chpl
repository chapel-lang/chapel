use Subprocess;
use Time;

var sub = spawn(["sleep", "60"]);
sleep(1);
sub.alarm();
while sub.running do
  sub.poll();
assert(sub.exitCode == -SIGALRM);
