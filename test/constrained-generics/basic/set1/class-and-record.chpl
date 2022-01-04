/*                                                                              
This is a variant of basic-arg-query.chpl                
that uses the same interface for a class and a record and a real,
where the required functions have ref and non-ref formals.                      
*/

interface IFC(T) {
  proc reqFun(ref f1: T, ref f2: T, f3: T);
  proc dfltImpl(ref f1: T, ref f2: T, f3: T) {
    writeln("in IFC.dfltImpl()");
  }
}
proc cgFun1(in a1: ?Q, ref a2: Q) where Q implements IFC {
  writeln("in cgFun1()");
  reqFun(a1, a2, a2);
  dfltImpl(a1, a2, a2);
}
proc cgFun2(ref a1: ?Q, in a2: Q) where Q implements IFC {
  writeln("in cgFun2()");
  reqFun(a1, a2, a2);
  dfltImpl(a1, a2, a2);
}

///////////

proc reqFun(ref i1: real, ref i2: real, i3: real) {
  writeln("in reqFun/real", (i1, i2, i3));
}
real implements IFC; // relies on the above reqFun(real)                        
var num = 4.5;
cgFun1(2.3, num);
cgFun2(num, 2.3);

///////////

record MyRecord { var x: int; }
proc reqFun(ref i1: MyRecord, ref i2: MyRecord, i3: MyRecord) {
  writeln("in reqFun/MyRecord", (i1.x, i2.x, i3.x));
}
MyRecord implements IFC; // relies on the above reqFun(real)                    
var rec = new MyRecord(45);
cgFun1(new MyRecord(23), rec);
cgFun2(new MyRecord(32), rec);

///////////

class MyClass { var x: int; }

proc reqFun(ref i1: MyClass, ref i2: MyClass, i3: MyClass) {
  writeln("in reqFun/MyClass!", (i1.x, i2.x, i3.x));
}
//inferred:  owned MyClass implements IFC;
var cls1 = new MyClass(45);
cgFun1(new MyClass(23), cls1);

proc reqFun(ref i1: MyClass?, ref i2: MyClass?, i3: MyClass?) {
  writeln("in reqFun/MyClass?", i1, i2, i3);
}
//inferred:  owned MyClass? implements IFC;
var cls2 = new MyClass(54)?;
cgFun2(new MyClass(32)?, cls2);
