// This example doesn't compile on master now even though
// it won't have any memory errors
class C { var x: int; }

proc test1() {
  var ownC = new owned C(1);
  var outerA: [1..1] borrowed C?;
  outerA[1] = ownC.borrow();

  {
    var innerA: [1..1] borrowed C?;
    innerA = outerA;

    outerA = innerA;
  }
}
test1();

proc test2() {
  var ownC = new owned C(1);
  var outerA: [1..1] borrowed C?;
  outerA[1] = ownC.borrow();

  {
    var innerA: [1..1] borrowed C?;
    innerA[1] = ownC.borrow();

    outerA[1] = innerA[1];
  }
}
test2();

proc test3() {
  var ownC = new owned C(1);
  var outerA: [1..1] borrowed C?;
  outerA[1] = ownC.borrow();

  {
    var innerA: [1..1] borrowed C?;
    innerA = outerA;

    outerA[1] = innerA[1];
  }
}
test3();

proc copy(a) {
  return a;
}

proc test4() {
  var ownC = new owned C(1);
  var outerA: [1..1] borrowed C?;
  outerA[1] = ownC.borrow();

  {
    var innerA = copy(outerA);

    outerA[1] = innerA[1];
  }
}
test4();
