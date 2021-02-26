class C { }

proc error() {
  // These are just never mentioned again
  var b1: borrowed C;
}
error();
