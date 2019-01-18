
// Based on #10080

record R1 {
  var a: R3;
}
record R2 {
  var a: R1;
}
record R3 {
  var a: R2;
}
proc main() {
  var x: R3;
  var y: [0..0] R3;
}
