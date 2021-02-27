// This code elaborates on recursive-ifc-1-error.chpl
// by defining THREE mutually-recursive interfaces.
//
// Within the recursive-ifc-3-* family,
// it checks whether we can report an "assoc. type is not defined" error.

interface Ifc1 {
  type AT1;
  AT1 implements Ifc2;
  proc reqFun1();
}

interface Ifc2 {
  type AT2;
  AT2 implements Ifc3;
  proc reqFun2();
}

interface Ifc3 {
  type AT3;
  AT3 implements Ifc1;
  proc reqFun3();
}

implements Ifc1(int(8));
implements Ifc2(int(16));
implements Ifc3(int(32)); // error: reqFun3 is not defined

proc integral.AT1 type return int(16);
proc integral.AT2 type return int(32);
proc integral.AT3 type return int(8);

proc reqFun1() { }
proc reqFun2() { }
//proc reqFun3() { }
