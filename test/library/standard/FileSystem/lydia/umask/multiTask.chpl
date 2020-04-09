use FileSystem;

var masks: [1..3] int = [0o777, 0o744, 0o755];

var startingMask = 0o700;
var oldMask = here.umask(startingMask);

serial {
  forall i in masks.domain {
    var prevMask = here.umask(masks[i]);
    // For every task after the first one, umask will be updated
    if (prevMask != startingMask) {
      // We should see this output twice.
      writeln("Not task safe!");
    }
  }
}
