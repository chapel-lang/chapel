enum log_level {
  none = 0,
  debug = 1
}

proc main {
  var e = log_level.debug;
  writeln(e);
}
