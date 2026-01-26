use List;
use Debugger;

record myRec {
  var x: int;
  var y: real;
}
class myClass {
  var a: int;
  var b: real;
}

proc createList(type listTy, in v1, in v2, in v3) {
  var myList = new listTy();
  myList.pushBack(v1);
  myList.pushBack(v2);
  myList.pushBack(v3);
  return myList;
}

proc main() {
  var myIntList = createList(list(int), 1, 2, 3);
  writeln("Integer List: ", myIntList);
  breakpoint;
  writeln("Integer List Again: ", myIntList);

  var myRealList = createList(list(real, false), 1.1, 2.2, 3.3);
  writeln("Real List: ", myRealList);
  breakpoint;
  writeln("Real List Again: ", myRealList);

  var myRecList = createList(list(myRec, true),
                              new myRec(10, 20.5),
                              new myRec(30, 40.5),
                              new myRec(50, 60.5));
  writeln("Record List: ", myRecList);
  breakpoint;
  writeln("Record List Again: ", myRecList);

  var myStrList = createList(list(string), "one", "two", "three");
  writeln("String List: ", myStrList);
  breakpoint;
  writeln("String List Again: ", myStrList);


  var myOwnedClassList = createList(list(owned myClass?, true),
                              new myClass(1, 1.1),
                              new myClass(2, 2.2),
                              new myClass(3, 3.3));
  writeln("Class List: ", myOwnedClassList);
  breakpoint;
  writeln("Class List Again: ", myOwnedClassList);


  var myClassList = createList(list(unmanaged myClass?),
                              new unmanaged myClass(1, 1.1),
                              new unmanaged myClass(2, 2.2),
                              new unmanaged myClass(3, 3.3));
  writeln("Class List: ", myClassList);
  breakpoint;
  writeln("Class List Again: ", myClassList);
}
