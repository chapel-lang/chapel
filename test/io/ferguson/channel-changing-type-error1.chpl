use IO;

proc main() {
  var f = openTempFile();
  var a = f.writer();
  var b:fileReader = a;
}
