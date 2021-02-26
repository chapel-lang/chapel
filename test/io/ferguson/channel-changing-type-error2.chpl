use IO;

proc main() {
  var f = opentmp();
  var a = f.reader();
  var b:channel(writing=true) = a;
}
