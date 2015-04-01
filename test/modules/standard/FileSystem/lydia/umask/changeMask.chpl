use FileSystem;

var newMask = 0o777;
var oldMask = here.umask(newMask);
var shouldMatchFirst = here.umask(oldMask);

if newMask != shouldMatchFirst {
  writeln("Uh oh, I didn't get the right umask back!");
  writef("Expected umask %oi but got %oi\n", newMask, shouldMatchFirst);
} else {
  writeln("Phew, the returned umask value matched what I sent it!");
}
