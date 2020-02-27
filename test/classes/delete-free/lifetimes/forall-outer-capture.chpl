class C { var x: int; }

proc error1() {
  var b: borrowed C?;
  {
    var o = new owned C();
    forall i in 1..10 with (ref b) {
      if i == 1 then
        b = o.borrow();
    }
  }
}
error1();

proc error2() {
  var b: borrowed C?;
  {
    var o = new owned C();
    var ob = o.borrow();
    forall i in 1..10 with (ref b) {
      if i == 1 then
        b = ob;
    }
  }
}
error2();
