use IO;

proc main() {
  var f = opentmp();
  var a = f.writer();
  var b:channel(writing=false,kind=iokind.dynamic,locking=true);
  b; // prevent split-init
  b = a;
}
