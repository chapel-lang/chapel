class pair {
  var x: int;
  var y: real;
}

proc main() {
  var a: unmanaged pair?;
  var aa: borrowed pair?;

  if (a == nil && aa == nil) {
    writeln("a initialized properly");
  }
}
