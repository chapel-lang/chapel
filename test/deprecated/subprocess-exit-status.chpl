use Subprocess;

proc main {
  var sub: subprocess(iokind.dynamic, false);
  var stat = sub.exit_status;
}
