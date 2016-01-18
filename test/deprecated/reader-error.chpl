class C {
  var x: int;
  var y: int;
  proc readThis(f:Reader) {
    f.readln("hello");
  }
  // Note that no readThis function will be generated.
}
var c = new C();
read(c);
