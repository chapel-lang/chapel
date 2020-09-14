use IO;

proc main() {
  var f = opentmp();
  var a = f.reader();
  var b:channel(writing=true,kind=iokind.dynamic,locking=true);
  b; // prevent split-init
  b = a;
}
