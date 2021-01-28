/*
This is basic-arg-query.chpl
where the required functions have ref and non-ref formals.
*/

interface IFC(T) {
  proc reqFun(ref f1: T, ref f2: T, f3: T);
  proc dfltImpl(ref f1: T, ref f2: T, f3: T) {
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(in a1: ?Q, ref a2: Q) where Q implements IFC {
  writeln("in cgFun()");
  reqFun(a1, a2, a2);
  dfltImpl(a1, a2, a2);
}

proc reqFun(ref i1: real, ref i2: real, i3: real) {
  writeln("in reqFun/real", (i1, i2, i3));
}

real implements IFC; // relies on the above reqFun(real)

var num = 4.5;
cgFun(2.3, num);

record MyRecord { var x: int; }

proc reqFun(ref i1: MyRecord, ref i2: MyRecord, i3: MyRecord) {
  writeln("in reqFun/MyRecord", (i1.x, i2.x, i3.x));
}

MyRecord implements IFC; // relies on the above reqFun(real)

var rec = new MyRecord(45);
cgFun(new MyRecord(23), rec);
