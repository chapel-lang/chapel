use IO;

proc main() {
  var f = openTempFile();
  var a = f.reader();
  var b:fileWriter(kind=iokind.dynamic,locking=true);
  b; // prevent split-init
  b = a;
}
