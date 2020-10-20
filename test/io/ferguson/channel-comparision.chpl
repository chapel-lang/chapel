use IO;

proc test1() {
  var f = opentmp();
  var a = f.reader();
  var b:channel(writing=false)=a;
  if b == a then
    writeln("Channels a and b are equal.");
}
test1();

proc test2() {
  var f = opentmp();
  var a = f.reader();
  var b:channel(writing=true,kind=iokind.dynamic,locking=true);
  b; // prevent split-init
  if b != a then
    writeln("Channels a and b are not equal.");
}
test2();