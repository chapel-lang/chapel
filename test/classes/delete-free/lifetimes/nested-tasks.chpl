use BlockDist;

class C { var x: int; }

proc test0() {
  var myc = new owned C(1);
  var bb: borrowed C?;

  coforall i in 1..2 with (ref bb) {
    bb = myc.borrow();
  }
}
test0();

proc test1() {
  var myc = new owned C(1);
  var bb: borrowed C?;

  coforall i in 1..2 with (ref bb) {
    coforall j in 1..2 with (ref bb) {
      bb = myc.borrow();
    }
  }
}
test1();

proc test1a() {
  var myc = new owned C(1);
  var from = myc.borrow();
  var bb: borrowed C?;

  coforall i in 1..2 with (ref bb) {
    var tmp = bb;
    coforall j in 1..2 with (ref tmp) {
      tmp = from;
    }
    bb = tmp;
  }
}
test1a();


proc test2() {
  var myc = new owned C(1);
  var bb: C = myc.borrow();

  coforall i in 1..2 {
    var tmp1 = bb;
    writeln(tmp1.x);
    coforall j in 1..2 {
      var tmp2 = tmp1;
      writeln(tmp2.x);
    }
  }
}
test2();

proc test3() {
  var myc = new owned C(2);
  var bb: C = myc.borrow();

  coforall i in 1..2 {
    var tmp = myc.borrow();
    ref local_bb = tmp;
    coforall j in 1..2 {
      var tmp2 = local_bb;
      writeln(tmp2.x);
    }
  }
}
test3();

proc test4() {

  const Space = {0..10};
  const D     = Space dmapped Block(Space);
  var A : [D] int;

  coforall loc in Locales do on loc {
    ref dstSlice = A.localSlice(A.localSubdomain());
    dstSlice = 1;
    coforall i in A.localSubdomain() {
      ref srcSlice = A.localSlice(A.localSubdomain());
      dstSlice[i] = srcSlice[i];
    }
  }
}
test4();
