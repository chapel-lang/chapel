use IO;

proc main() {
  var f = opentmp();
  var a = f.writer();
  var b:channel(writing=false) = a;
}
