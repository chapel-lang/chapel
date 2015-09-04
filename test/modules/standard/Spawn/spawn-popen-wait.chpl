use Spawn;
use Time;

config const showtime = false;

/* This test reads lines at a time from
   a subprocess that is sleeping. The
   purpose of the test it to check that
   the pipe from the subprocess is live -
   that we get the subprocess output
   in a reasonable amount of time.

   For that reason, it depends on timing.
   It's technically possible for this
   test to fail spuriously if a machine
   goes out to lunch for 5 seconds.
 */

var timer:Timer;

var sub = spawn(["bash", "10.bash"], stdout=PIPE);

timer.start();

var line:string;
var i = 0;
while sub.stdout.readline(line) {
  write("Got line: ", line);

  // We want to make sure we're not just buffering everything
  // up and reading it at once.

  // Make sure no more than i+5 seconds have elapsed.
  var elapsed = timer.elapsed();
  if showtime then writeln(elapsed, " seconds");
  assert(elapsed <= i+5);
  i += 1;
}


sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


