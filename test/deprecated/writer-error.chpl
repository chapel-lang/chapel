class C {
  var x: int;
  var y: int;
  proc writeThis(f:Writer) {
    f.writeln("hello");
  }
  // Note that no readThis function will be generated.
}
var c = new C();
write(c);
