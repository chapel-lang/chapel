
class C {
  var x: int;
}

proc +(a: borrowed C, b: borrowed C) {
  return a;
}
proc +(a: borrowed C?, b: borrowed C?) {
  return b;
}


var a = (new owned C(1)).borrow();
var bq:borrowed C? = (new owned C(2)).borrow();

// how is a + bq! parsed?
//   a + (bq!)
//   (a + bq)!

writeln(a + bq!);   // expect 1
writeln(a + (bq!)); // expect 1
writeln((a + bq)!); // expect 2
writeln(a + bq);    // expect 2
