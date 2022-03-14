config const n = 50;

proc foo(i) {
  if i > 10 then
    halt("i is too big!");
}

on here.getChild(1) {
  var A: [0..#n] real;
  forall i in 0..#n {
    foo(i);
  }
}
