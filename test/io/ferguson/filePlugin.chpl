// Just make sure that file.filePlugin compiles
proc main() {
  use IO;
  var f = openTempFile();
  var x = f.filePlugin();
}
