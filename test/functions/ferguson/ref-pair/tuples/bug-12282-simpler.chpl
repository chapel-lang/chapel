record R1 {
  var x: int;
}

record R2 {
  var x: 3*R1;
}

const myConstR2: R2;

proc main() {
  var myR2: R2;

  myR2 = myConstR2;
}
