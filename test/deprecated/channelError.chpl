use IO, SysBasic;

proc main() {
  if stdout.error() then writeln("Impossible!");

  var error: syserr = EEOF;
  stdout.setError(error);

  stdout.clearError();
}

