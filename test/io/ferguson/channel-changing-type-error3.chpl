use IO;

proc main() {
  var f = openTempFile();
  var a = f.writer();
  var b:fileReader(kind=iokind.dynamic,locking=true);
  b; // prevent split-init
  b = a;
}
