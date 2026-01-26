use Map;
use Debugger;

record myRec {
  var x: int;
  var y: real;
}
class myClass {
  var a: int;
  var b: real;
}

proc createMap(type mapTy, in k1, in v1, in k2, in v2, in k3, in v3) {
  var myMap = new mapTy();
  myMap.add(k1, v1);
  myMap.add(k2, v2);
  myMap.add(k3, v3);
  return myMap;
}

proc main() {
  var myIntMap = createMap(map(int, int), 1, 11, 3, 13, 6, 16);
  writeln("Integer Map: ", myIntMap);
  breakpoint;
  writeln("Integer Map Again: ", myIntMap);

  var myRealMap = createMap(map(string, real, false), "1.1", 1.1, "2.2", 2.2, "3.3", 3.3);
  writeln("Real Map: ", myRealMap);
  breakpoint;
  writeln("Real Map Again: ", myRealMap);

  var myRecMap = createMap(map(int, myRec, true),
                            1, new myRec(10, 20.5),
                            2, new myRec(30, 40.5),
                            3, new myRec(50, 60.5));
  writeln("Record Map: ", myRecMap);
  breakpoint;
  writeln("Record Map Again: ", myRecMap);

  var myStrMap = createMap(map(string, string), "1", "one", "2", "two", "3", "three");
  writeln("String Map: ", myStrMap);
  breakpoint;
  writeln("String Map Again: ", myStrMap);

  var myOwnedClassMap = createMap(map(string, owned myClass?, true),
                              "first", new myClass(1, 1.1),
                              "second", new myClass(2, 2.2),
                              "third", new myClass(3, 3.3));
  writeln("Class Map: ", myOwnedClassMap);
  breakpoint;
  writeln("Class Map Again: ", myOwnedClassMap);

  var myClassMap = createMap(map(int, unmanaged myClass?, true),
                              1, new unmanaged myClass(1, 1.1),
                              2, new unmanaged myClass(2, 2.2),
                              3, new unmanaged myClass(3, 3.3));
  writeln("Class Map: ", myClassMap);
  breakpoint;
  writeln("Class Map Again: ", myClassMap);
}
