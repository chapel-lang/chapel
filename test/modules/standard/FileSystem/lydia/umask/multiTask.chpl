use FileSystem;

var masks: [1..3] int = [0o777, 0o744, 0o755];

var startingMask = 0o700;
var oldMask = here.umask(startingMask);

forall i in masks.domain {
  var prevMask = here.umask(masks[i]);
  if (prevMask != startingMask) {
    writeln("Not task safe!");
  }
}