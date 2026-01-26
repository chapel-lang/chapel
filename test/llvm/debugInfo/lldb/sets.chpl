use Set;
use Debugger;

record myRec {
  var x: int;
  var y: real;
}
class myClass {
  var a: int;
  var b: real;
}

proc createSet(type setTy, in v1, in v2, in v3) {
  var mySet = new setTy();
  mySet.add(v1);
  mySet.add(v2);
  mySet.add(v3);
  return mySet;
}

proc main() {
  var myIntSet = createSet(set(int), 1, 2, 3);
  writeln("Integer Set: ", myIntSet);
  breakpoint;
  writeln("Integer Set Again: ", myIntSet);

  var myRealSet = createSet(set(real, false), 1.1, 2.2, 3.3);
  writeln("Real Set: ", myRealSet);
  breakpoint;
  writeln("Real Set Again: ", myRealSet);

  var myRecSet = createSet(set(myRec, true),
                            new myRec(10, 20.5),
                            new myRec(30, 40.5),
                            new myRec(50, 60.5));
  writeln("Record Set: ", myRecSet);
  breakpoint;
  writeln("Record Set Again: ", myRecSet);

  var myStrSet = createSet(set(string), "one", "two", "three");
  writeln("String Set: ", myStrSet);
  breakpoint;
  writeln("String Set Again: ", myStrSet);

  var myOwnedClassSet = createSet(set(owned myClass?, true),
                              new myClass(1, 1.1),
                              new myClass(2, 2.2),
                              new myClass(3, 3.3));
  writeln("Class Set: ", myOwnedClassSet);
  breakpoint;
  writeln("Class Set Again: ", myOwnedClassSet);


  var myClassSet = createSet(set(unmanaged myClass?),
                              new unmanaged myClass(1, 1.1),
                              new unmanaged myClass(2, 2.2),
                              new unmanaged myClass(3, 3.3));
  writeln("Class Set: ", myClassSet);
  breakpoint;
  writeln("Class Set Again: ", myClassSet);
}

