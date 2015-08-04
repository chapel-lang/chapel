use Subprocess;
use Time;

config const showtime = false;

var timer:Timer;

var sub = spawn(["bash", "10.bash"], stdout=PIPE);

timer.start();

var line:string;
var i = 0;
while sub.stdout.readline(line) {
  write("Got line: ", line);

  // We want to make sure we're not just buffering everything
  // up and reading it at once.

  // Make sure no more than i+2 seconds have elapsed.
  var elapsed = timer.elapsed();
  if showtime then writeln(elapsed, " seconds");
  assert(elapsed <= i+2);
  i += 1;
}


sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


