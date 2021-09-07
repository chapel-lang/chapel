use Spawn;

var sub = spawn(["./junk"]);

// Different systems report this kind of error
// at different times. On Mac OS X, an error
// will be thrown. On Linux, exitCodeUNIQUE != 0.
try {
  sub.wait();
  assert(sub.running == false);
  assert(sub.exitCodeUNIQUE != 0);
} catch {
  assert(sub.running == false);
}

writeln("successfully handled error");
sub.close();
