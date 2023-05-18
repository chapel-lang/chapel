module T1 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to owned A
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:owned A;
    myError("KEEP ME: owned A to owned A is legal");
  }
}
module T2 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to owned Parent
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:owned Parent;
    myError("KEEP ME: owned Child to owned Parent is legal");
  }
}
module T3 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to owned Child
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:owned Child;
    myError("KEEP ME: owned Parent to owned Child is legal");
  }
}
module T4 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to shared A
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:shared A;
    myError("KEEP ME: owned A to shared A is legal");
  }
}
module T5 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to shared Parent
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:shared Parent;
  }
}
module T6 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to shared Child
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:shared Child;
  }
}
module T7 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to unmanaged A
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: owned A to unmanaged A is legal");
  }
}
module T8 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to unmanaged Parent
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: owned Child to unmanaged Parent is legal");
  }
}
module T9 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to unmanaged Child
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: owned Parent to unmanaged Child is legal");
  }
}
module T10 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to borrowed A
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: owned A to borrowed A is legal");
  }
}
module T11 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to borrowed Parent
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: owned Child to borrowed Parent is legal");
  }
}
module T12 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to borrowed Child
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: owned Parent to borrowed Child is legal");
  }
}
module T13 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to owned A?
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:owned A?;
    myError("KEEP ME: owned A to owned A? is legal");
  }
}
module T14 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to owned Parent?
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:owned Parent?;
    myError("KEEP ME: owned Child to owned Parent? is legal");
  }
}
module T15 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to owned Child?
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:owned Child?;
    myError("KEEP ME: owned Parent to owned Child? is legal");
  }
}
module T16 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to shared A?
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:shared A?;
  }
}
module T17 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to shared Parent?
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:shared Parent?;
  }
}
module T18 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to shared Child?
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:shared Child?;
  }
}
module T19 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to unmanaged A?
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: owned A to unmanaged A? is legal");
  }
}
module T20 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to unmanaged Parent?
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: owned Child to unmanaged Parent? is legal");
  }
}
module T21 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to unmanaged Child?
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: owned Parent to unmanaged Child? is legal");
  }
}
module T22 {
  use explicitCasts;
  proc foo() {
    // casting from owned A to borrowed A?
    var alloc = new owned A();
    var a:owned A = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: owned A to borrowed A? is legal");
  }
}
module T23 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child to borrowed Parent?
    var alloc = new owned Child();
    var a:owned Child = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: owned Child to borrowed Parent? is legal");
  }
}
module T24 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent to borrowed Child?
    var alloc = new owned Parent();
    var a:owned Parent = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: owned Parent to borrowed Child? is legal");
  }
}
module T25 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to owned A
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:owned A;
  }
}
module T26 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to owned Parent
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:owned Parent;
  }
}
module T27 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to owned Child
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:owned Child;
  }
}
module T28 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to shared A
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:shared A;
    myError("KEEP ME: shared A to shared A is legal");
  }
}
module T29 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to shared Parent
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:shared Parent;
    myError("KEEP ME: shared Child to shared Parent is legal");
  }
}
module T30 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to shared Child
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:shared Child;
    myError("KEEP ME: shared Parent to shared Child is legal");
  }
}
module T31 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to unmanaged A
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: shared A to unmanaged A is legal");
  }
}
module T32 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to unmanaged Parent
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: shared Child to unmanaged Parent is legal");
  }
}
module T33 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to unmanaged Child
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: shared Parent to unmanaged Child is legal");
  }
}
module T34 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to borrowed A
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: shared A to borrowed A is legal");
  }
}
module T35 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to borrowed Parent
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: shared Child to borrowed Parent is legal");
  }
}
module T36 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to borrowed Child
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: shared Parent to borrowed Child is legal");
  }
}
module T37 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to owned A?
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:owned A?;
  }
}
module T38 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to owned Parent?
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:owned Parent?;
  }
}
module T39 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to owned Child?
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:owned Child?;
  }
}
module T40 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to shared A?
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:shared A?;
    myError("KEEP ME: shared A to shared A? is legal");
  }
}
module T41 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to shared Parent?
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:shared Parent?;
    myError("KEEP ME: shared Child to shared Parent? is legal");
  }
}
module T42 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to shared Child?
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:shared Child?;
    myError("KEEP ME: shared Parent to shared Child? is legal");
  }
}
module T43 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to unmanaged A?
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: shared A to unmanaged A? is legal");
  }
}
module T44 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to unmanaged Parent?
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: shared Child to unmanaged Parent? is legal");
  }
}
module T45 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to unmanaged Child?
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: shared Parent to unmanaged Child? is legal");
  }
}
module T46 {
  use explicitCasts;
  proc foo() {
    // casting from shared A to borrowed A?
    var alloc = new shared A();
    var a:shared A = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: shared A to borrowed A? is legal");
  }
}
module T47 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child to borrowed Parent?
    var alloc = new shared Child();
    var a:shared Child = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: shared Child to borrowed Parent? is legal");
  }
}
module T48 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent to borrowed Child?
    var alloc = new shared Parent();
    var a:shared Parent = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: shared Parent to borrowed Child? is legal");
  }
}
module T49 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to owned A
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:owned A;
  }
}
module T50 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to owned Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:owned Parent;
  }
}
module T51 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to owned Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:owned Child;
  }
}
module T52 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to shared A
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:shared A;
  }
}
module T53 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to shared Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:shared Parent;
  }
}
module T54 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to shared Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:shared Child;
  }
}
module T55 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to unmanaged A
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: unmanaged A to unmanaged A is legal");
  }
}
module T56 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to unmanaged Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: unmanaged Child to unmanaged Parent is legal");
  }
}
module T57 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to unmanaged Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: unmanaged Parent to unmanaged Child is legal");
  }
}
module T58 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to borrowed A
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: unmanaged A to borrowed A is legal");
  }
}
module T59 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to borrowed Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: unmanaged Child to borrowed Parent is legal");
  }
}
module T60 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to borrowed Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: unmanaged Parent to borrowed Child is legal");
  }
}
module T61 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to owned A?
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:owned A?;
  }
}
module T62 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to owned Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:owned Parent?;
  }
}
module T63 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to owned Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:owned Child?;
  }
}
module T64 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to shared A?
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:shared A?;
  }
}
module T65 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to shared Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:shared Parent?;
  }
}
module T66 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to shared Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:shared Child?;
  }
}
module T67 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to unmanaged A?
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: unmanaged A to unmanaged A? is legal");
  }
}
module T68 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to unmanaged Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: unmanaged Child to unmanaged Parent? is legal");
  }
}
module T69 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to unmanaged Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: unmanaged Parent to unmanaged Child? is legal");
  }
}
module T70 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A to borrowed A?
    var alloc = new unmanaged A();
    var a:unmanaged A = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: unmanaged A to borrowed A? is legal");
  }
}
module T71 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child to borrowed Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: unmanaged Child to borrowed Parent? is legal");
  }
}
module T72 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent to borrowed Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: unmanaged Parent to borrowed Child? is legal");
  }
}
module T73 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to owned A
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:owned A;
  }
}
module T74 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to owned Parent
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:owned Parent;
  }
}
module T75 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to owned Child
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:owned Child;
  }
}
module T76 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to shared A
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:shared A;
  }
}
module T77 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to shared Parent
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:shared Parent;
  }
}
module T78 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to shared Child
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:shared Child;
  }
}
module T79 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to unmanaged A
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: borrowed A to unmanaged A is legal");
  }
}
module T80 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to unmanaged Parent
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: borrowed Child to unmanaged Parent is legal");
  }
}
module T81 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to unmanaged Child
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: borrowed Parent to unmanaged Child is legal");
  }
}
module T82 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to borrowed A
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: borrowed A to borrowed A is legal");
  }
}
module T83 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to borrowed Parent
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: borrowed Child to borrowed Parent is legal");
  }
}
module T84 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to borrowed Child
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: borrowed Parent to borrowed Child is legal");
  }
}
module T85 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to owned A?
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:owned A?;
  }
}
module T86 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to owned Parent?
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:owned Parent?;
  }
}
module T87 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to owned Child?
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:owned Child?;
  }
}
module T88 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to shared A?
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:shared A?;
  }
}
module T89 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to shared Parent?
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:shared Parent?;
  }
}
module T90 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to shared Child?
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:shared Child?;
  }
}
module T91 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to unmanaged A?
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: borrowed A to unmanaged A? is legal");
  }
}
module T92 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to unmanaged Parent?
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: borrowed Child to unmanaged Parent? is legal");
  }
}
module T93 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to unmanaged Child?
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: borrowed Parent to unmanaged Child? is legal");
  }
}
module T94 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A to borrowed A?
    var alloc = new owned A();
    var a:borrowed A = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: borrowed A to borrowed A? is legal");
  }
}
module T95 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child to borrowed Parent?
    var alloc = new owned Child();
    var a:borrowed Child = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: borrowed Child to borrowed Parent? is legal");
  }
}
module T96 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent to borrowed Child?
    var alloc = new owned Parent();
    var a:borrowed Parent = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: borrowed Parent to borrowed Child? is legal");
  }
}
module T97 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to owned A
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:owned A;
    myError("KEEP ME: owned A? to owned A is legal");
  }
}
module T98 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to owned Parent
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:owned Parent;
    myError("KEEP ME: owned Child? to owned Parent is legal");
  }
}
module T99 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to owned Child
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:owned Child;
    myError("KEEP ME: owned Parent? to owned Child is legal");
  }
}
module T100 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to shared A
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:shared A;
  }
}
module T101 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to shared Parent
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:shared Parent;
  }
}
module T102 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to shared Child
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:shared Child;
  }
}
module T103 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to unmanaged A
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: owned A? to unmanaged A is legal");
  }
}
module T104 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to unmanaged Parent
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: owned Child? to unmanaged Parent is legal");
  }
}
module T105 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to unmanaged Child
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: owned Parent? to unmanaged Child is legal");
  }
}
module T106 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to borrowed A
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: owned A? to borrowed A is legal");
  }
}
module T107 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to borrowed Parent
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: owned Child? to borrowed Parent is legal");
  }
}
module T108 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to borrowed Child
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: owned Parent? to borrowed Child is legal");
  }
}
module T109 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to owned A?
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:owned A?;
    myError("KEEP ME: owned A? to owned A? is legal");
  }
}
module T110 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to owned Parent?
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:owned Parent?;
    myError("KEEP ME: owned Child? to owned Parent? is legal");
  }
}
module T111 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to owned Child?
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:owned Child?;
    myError("KEEP ME: owned Parent? to owned Child? is legal");
  }
}
module T112 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to shared A?
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:shared A?;
    myError("KEEP ME: owned A? to shared A? is legal");
  }
}
module T113 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to shared Parent?
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:shared Parent?;
  }
}
module T114 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to shared Child?
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:shared Child?;
  }
}
module T115 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to unmanaged A?
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: owned A? to unmanaged A? is legal");
  }
}
module T116 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to unmanaged Parent?
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: owned Child? to unmanaged Parent? is legal");
  }
}
module T117 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to unmanaged Child?
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: owned Parent? to unmanaged Child? is legal");
  }
}
module T118 {
  use explicitCasts;
  proc foo() {
    // casting from owned A? to borrowed A?
    var alloc = new owned A();
    var a:owned A? = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: owned A? to borrowed A? is legal");
  }
}
module T119 {
  use explicitCasts;
  proc foo() {
    // casting from owned Child? to borrowed Parent?
    var alloc = new owned Child();
    var a:owned Child? = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: owned Child? to borrowed Parent? is legal");
  }
}
module T120 {
  use explicitCasts;
  proc foo() {
    // casting from owned Parent? to borrowed Child?
    var alloc = new owned Parent();
    var a:owned Parent? = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: owned Parent? to borrowed Child? is legal");
  }
}
module T121 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to owned A
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:owned A;
  }
}
module T122 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to owned Parent
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:owned Parent;
  }
}
module T123 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to owned Child
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:owned Child;
  }
}
module T124 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to shared A
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:shared A;
    myError("KEEP ME: shared A? to shared A is legal");
  }
}
module T125 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to shared Parent
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:shared Parent;
    myError("KEEP ME: shared Child? to shared Parent is legal");
  }
}
module T126 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to shared Child
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:shared Child;
    myError("KEEP ME: shared Parent? to shared Child is legal");
  }
}
module T127 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to unmanaged A
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: shared A? to unmanaged A is legal");
  }
}
module T128 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to unmanaged Parent
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: shared Child? to unmanaged Parent is legal");
  }
}
module T129 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to unmanaged Child
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: shared Parent? to unmanaged Child is legal");
  }
}
module T130 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to borrowed A
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: shared A? to borrowed A is legal");
  }
}
module T131 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to borrowed Parent
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: shared Child? to borrowed Parent is legal");
  }
}
module T132 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to borrowed Child
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: shared Parent? to borrowed Child is legal");
  }
}
module T133 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to owned A?
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:owned A?;
  }
}
module T134 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to owned Parent?
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:owned Parent?;
  }
}
module T135 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to owned Child?
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:owned Child?;
  }
}
module T136 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to shared A?
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:shared A?;
    myError("KEEP ME: shared A? to shared A? is legal");
  }
}
module T137 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to shared Parent?
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:shared Parent?;
    myError("KEEP ME: shared Child? to shared Parent? is legal");
  }
}
module T138 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to shared Child?
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:shared Child?;
    myError("KEEP ME: shared Parent? to shared Child? is legal");
  }
}
module T139 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to unmanaged A?
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: shared A? to unmanaged A? is legal");
  }
}
module T140 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to unmanaged Parent?
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: shared Child? to unmanaged Parent? is legal");
  }
}
module T141 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to unmanaged Child?
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: shared Parent? to unmanaged Child? is legal");
  }
}
module T142 {
  use explicitCasts;
  proc foo() {
    // casting from shared A? to borrowed A?
    var alloc = new shared A();
    var a:shared A? = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: shared A? to borrowed A? is legal");
  }
}
module T143 {
  use explicitCasts;
  proc foo() {
    // casting from shared Child? to borrowed Parent?
    var alloc = new shared Child();
    var a:shared Child? = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: shared Child? to borrowed Parent? is legal");
  }
}
module T144 {
  use explicitCasts;
  proc foo() {
    // casting from shared Parent? to borrowed Child?
    var alloc = new shared Parent();
    var a:shared Parent? = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: shared Parent? to borrowed Child? is legal");
  }
}
module T145 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to owned A
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:owned A;
  }
}
module T146 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to owned Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:owned Parent;
  }
}
module T147 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to owned Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:owned Child;
  }
}
module T148 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to shared A
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:shared A;
  }
}
module T149 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to shared Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:shared Parent;
  }
}
module T150 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to shared Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:shared Child;
  }
}
module T151 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to unmanaged A
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: unmanaged A? to unmanaged A is legal");
  }
}
module T152 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to unmanaged Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: unmanaged Child? to unmanaged Parent is legal");
  }
}
module T153 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to unmanaged Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: unmanaged Parent? to unmanaged Child is legal");
  }
}
module T154 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to borrowed A
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: unmanaged A? to borrowed A is legal");
  }
}
module T155 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to borrowed Parent
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: unmanaged Child? to borrowed Parent is legal");
  }
}
module T156 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to borrowed Child
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: unmanaged Parent? to borrowed Child is legal");
  }
}
module T157 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to owned A?
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:owned A?;
  }
}
module T158 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to owned Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:owned Parent?;
  }
}
module T159 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to owned Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:owned Child?;
  }
}
module T160 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to shared A?
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:shared A?;
  }
}
module T161 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to shared Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:shared Parent?;
  }
}
module T162 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to shared Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:shared Child?;
  }
}
module T163 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to unmanaged A?
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: unmanaged A? to unmanaged A? is legal");
  }
}
module T164 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to unmanaged Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: unmanaged Child? to unmanaged Parent? is legal");
  }
}
module T165 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to unmanaged Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: unmanaged Parent? to unmanaged Child? is legal");
  }
}
module T166 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged A? to borrowed A?
    var alloc = new unmanaged A();
    var a:unmanaged A? = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: unmanaged A? to borrowed A? is legal");
  }
}
module T167 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Child? to borrowed Parent?
    var alloc = new unmanaged Child();
    var a:unmanaged Child? = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: unmanaged Child? to borrowed Parent? is legal");
  }
}
module T168 {
  use explicitCasts;
  proc foo() {
    // casting from unmanaged Parent? to borrowed Child?
    var alloc = new unmanaged Parent();
    var a:unmanaged Parent? = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: unmanaged Parent? to borrowed Child? is legal");
  }
}
module T169 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to owned A
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:owned A;
  }
}
module T170 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to owned Parent
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:owned Parent;
  }
}
module T171 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to owned Child
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:owned Child;
  }
}
module T172 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to shared A
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:shared A;
  }
}
module T173 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to shared Parent
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:shared Parent;
  }
}
module T174 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to shared Child
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:shared Child;
  }
}
module T175 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to unmanaged A
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:unmanaged A;
    myError("KEEP ME: borrowed A? to unmanaged A is legal");
  }
}
module T176 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to unmanaged Parent
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:unmanaged Parent;
    myError("KEEP ME: borrowed Child? to unmanaged Parent is legal");
  }
}
module T177 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to unmanaged Child
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:unmanaged Child;
    myError("KEEP ME: borrowed Parent? to unmanaged Child is legal");
  }
}
module T178 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to borrowed A
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:borrowed A;
    myError("KEEP ME: borrowed A? to borrowed A is legal");
  }
}
module T179 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to borrowed Parent
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:borrowed Parent;
    myError("KEEP ME: borrowed Child? to borrowed Parent is legal");
  }
}
module T180 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to borrowed Child
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:borrowed Child;
    myError("KEEP ME: borrowed Parent? to borrowed Child is legal");
  }
}
module T181 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to owned A?
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:owned A?;
  }
}
module T182 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to owned Parent?
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:owned Parent?;
  }
}
module T183 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to owned Child?
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:owned Child?;
  }
}
module T184 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to shared A?
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:shared A?;
  }
}
module T185 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to shared Parent?
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:shared Parent?;
  }
}
module T186 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to shared Child?
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:shared Child?;
  }
}
module T187 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to unmanaged A?
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:unmanaged A?;
    myError("KEEP ME: borrowed A? to unmanaged A? is legal");
  }
}
module T188 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to unmanaged Parent?
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:unmanaged Parent?;
    myError("KEEP ME: borrowed Child? to unmanaged Parent? is legal");
  }
}
module T189 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to unmanaged Child?
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:unmanaged Child?;
    myError("KEEP ME: borrowed Parent? to unmanaged Child? is legal");
  }
}
module T190 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed A? to borrowed A?
    var alloc = new owned A();
    var a:borrowed A? = alloc;
    var a_ = a:borrowed A?;
    myError("KEEP ME: borrowed A? to borrowed A? is legal");
  }
}
module T191 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Child? to borrowed Parent?
    var alloc = new owned Child();
    var a:borrowed Child? = alloc;
    var a_ = a:borrowed Parent?;
    myError("KEEP ME: borrowed Child? to borrowed Parent? is legal");
  }
}
module T192 {
  use explicitCasts;
  proc foo() {
    // casting from borrowed Parent? to borrowed Child?
    var alloc = new owned Parent();
    var a:borrowed Parent? = alloc;
    var a_ = a:borrowed Child?;
    myError("KEEP ME: borrowed Parent? to borrowed Child? is legal");
  }
}
class A {}
class Parent {}
class Child : Parent {}
proc myError(param a: string) { compilerError(a); }
proc main() {
  T1.foo();
  T2.foo();
  T3.foo();
  T4.foo();
  T5.foo();
  T6.foo();
  T7.foo();
  T8.foo();
  T9.foo();
  T10.foo();
  T11.foo();
  T12.foo();
  T13.foo();
  T14.foo();
  T15.foo();
  T16.foo();
  T17.foo();
  T18.foo();
  T19.foo();
  T20.foo();
  T21.foo();
  T22.foo();
  T23.foo();
  T24.foo();
  T25.foo();
  T26.foo();
  T27.foo();
  T28.foo();
  T29.foo();
  T30.foo();
  T31.foo();
  T32.foo();
  T33.foo();
  T34.foo();
  T35.foo();
  T36.foo();
  T37.foo();
  T38.foo();
  T39.foo();
  T40.foo();
  T41.foo();
  T42.foo();
  T43.foo();
  T44.foo();
  T45.foo();
  T46.foo();
  T47.foo();
  T48.foo();
  T49.foo();
  T50.foo();
  T51.foo();
  T52.foo();
  T53.foo();
  T54.foo();
  T55.foo();
  T56.foo();
  T57.foo();
  T58.foo();
  T59.foo();
  T60.foo();
  T61.foo();
  T62.foo();
  T63.foo();
  T64.foo();
  T65.foo();
  T66.foo();
  T67.foo();
  T68.foo();
  T69.foo();
  T70.foo();
  T71.foo();
  T72.foo();
  T73.foo();
  T74.foo();
  T75.foo();
  T76.foo();
  T77.foo();
  T78.foo();
  T79.foo();
  T80.foo();
  T81.foo();
  T82.foo();
  T83.foo();
  T84.foo();
  T85.foo();
  T86.foo();
  T87.foo();
  T88.foo();
  T89.foo();
  T90.foo();
  T91.foo();
  T92.foo();
  T93.foo();
  T94.foo();
  T95.foo();
  T96.foo();
  T97.foo();
  T98.foo();
  T99.foo();
  T100.foo();
  T101.foo();
  T102.foo();
  T103.foo();
  T104.foo();
  T105.foo();
  T106.foo();
  T107.foo();
  T108.foo();
  T109.foo();
  T110.foo();
  T111.foo();
  T112.foo();
  T113.foo();
  T114.foo();
  T115.foo();
  T116.foo();
  T117.foo();
  T118.foo();
  T119.foo();
  T120.foo();
  T121.foo();
  T122.foo();
  T123.foo();
  T124.foo();
  T125.foo();
  T126.foo();
  T127.foo();
  T128.foo();
  T129.foo();
  T130.foo();
  T131.foo();
  T132.foo();
  T133.foo();
  T134.foo();
  T135.foo();
  T136.foo();
  T137.foo();
  T138.foo();
  T139.foo();
  T140.foo();
  T141.foo();
  T142.foo();
  T143.foo();
  T144.foo();
  T145.foo();
  T146.foo();
  T147.foo();
  T148.foo();
  T149.foo();
  T150.foo();
  T151.foo();
  T152.foo();
  T153.foo();
  T154.foo();
  T155.foo();
  T156.foo();
  T157.foo();
  T158.foo();
  T159.foo();
  T160.foo();
  T161.foo();
  T162.foo();
  T163.foo();
  T164.foo();
  T165.foo();
  T166.foo();
  T167.foo();
  T168.foo();
  T169.foo();
  T170.foo();
  T171.foo();
  T172.foo();
  T173.foo();
  T174.foo();
  T175.foo();
  T176.foo();
  T177.foo();
  T178.foo();
  T179.foo();
  T180.foo();
  T181.foo();
  T182.foo();
  T183.foo();
  T184.foo();
  T185.foo();
  T186.foo();
  T187.foo();
  T188.foo();
  T189.foo();
  T190.foo();
  T191.foo();
  T192.foo();
}
