use IO;

proc foo(arg: channel) { }

proc main() {
  var f = openTempFile();

  var r: channel = f.reader();
  var w: channel = f.writer();

  foo(r);
  foo(w);
}
