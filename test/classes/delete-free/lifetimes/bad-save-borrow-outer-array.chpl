pragma "safe"
module BadSaveBorrowInOuterArray {

class C { }


proc error1() {
  var c: [1..3] borrowed C?;
  for i in 1..3 {
    c[i] = new owned C();
  }
}
error1();

proc error2() {
  var c: [1..3] borrowed C?;
  for i in 1..3 {
    var myowned = new owned C();
    c[i] = myowned;
  }
}
error2();

proc error3() {
  var c: [1..3] borrowed C?;
  for i in 1..3 {
    var myowned = new owned C();
    c[i] = myowned.borrow();
  }
}
error3();

}
