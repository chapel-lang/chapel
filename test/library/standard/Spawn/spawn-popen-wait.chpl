use Subprocess;
use Time;
use IO;

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

var timer:stopwatch;

var sub = spawn(["bash", "waiting.bash"], stdout=pipeStyle.pipe);

timer.start();

config const wait = 6;
config const debug = false;

var line:string;
var i = 0;
var signaled = false;
var signalFile = "";
var lastline = "";
while sub.stdout.readLine(line) {
  line = line.strip();

  if line != lastline && line != "Waiting" {
    writeln(line);
  }

  if debug {
    var elapsed = timer.elapsed();
    writeln(elapsed, " line: ", line);
  }

  if signalFile == "" {
    signalFile = line;
  }

  if i >= wait && !signaled {
    if debug {
      writeln("writing to ", signalFile);
    }
    signaled = true;
    var f = open(signalFile, ioMode.cw);
    var ch = f.writer();
    ch.writeln();
    ch.close();
    f.close();
  }

  lastline = line;
  i += 1;
}


sub.wait();
assert(sub.running == false);
assert(sub.exitCode == 0);

sub.close();
