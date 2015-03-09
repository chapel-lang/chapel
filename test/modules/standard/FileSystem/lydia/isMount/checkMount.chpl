use FileSystem;

var knownMount = "/lus/scratch";
if (isMount(knownMount)) {
  // The known mount point was detected as a mount point.  Phew.
  writeln("okay");
} else {
  writeln("Should have detected that " + knownMount + " was a mount point");
}
