use Spawn;

var sub = spawn(["./junk"]);

try {
  sub.wait();
} catch {
  assert(sub.running == false);

  // Different systems report this kind of error
  // at different times. On Mac OS X, error != 0,
  // but on Linux, exit_status != 0.
  assert(sub.exit_status != 0);

  writeln("successfully handled error");
}

sub.close();
