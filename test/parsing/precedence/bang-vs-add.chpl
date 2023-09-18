
class C {
  var x: int;
}

operator C.+(a: borrowed C, b: borrowed C) {
  return a;
}
operator C.+(a: borrowed C?, b: borrowed C?) {
  return b;
}


var ownA = new owned C(1);
var a = ownA.borrow();
var ownBq = new owned C(2);
var bq:borrowed C? = ownBq.borrow();

// how is a + bq! parsed?
//   a + (bq!)
//   (a + bq)!

writeln(a + bq!);   // expect 1
writeln(a + (bq!)); // expect 1
writeln((a + bq)!); // expect 2
writeln(a + bq);    // expect 2
