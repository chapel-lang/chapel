use Map;
use Debugger;

record myRec {
  var x: int;
  var y: real;
}
class MyClass {
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

  var myOwnedClassMap = createMap(map(string, owned MyClass?, true),
                              "first", new MyClass(1, 1.1),
                              "second", new MyClass(2, 2.2),
                              "third", new MyClass(3, 3.3));
  writeln("Class Map: ", myOwnedClassMap);
  breakpoint;
  writeln("Class Map Again: ", myOwnedClassMap);

  var myClassMap = createMap(map(int, unmanaged MyClass?, true),
                              1, new unmanaged MyClass(1, 1.1),
                              2, new unmanaged MyClass(2, 2.2),
                              3, new unmanaged MyClass(3, 3.3));
  writeln("Class Map: ", myClassMap);
  breakpoint;
  writeln("Class Map Again: ", myClassMap);
  for x in myClassMap.values() do delete x;
}


// CHECK: p myIntMap
// CHECK-NEXT: (Map::map(int(64),int(64)
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = (key = 1, val = 11)
// CHECK-NEXT: [1] = (key = 6, val = 16)
// CHECK-NEXT: [2] = (key = 3, val = 13)

// CHECK: v myIntMap[1]
// CHECK: = (key = 6, val = 16)

// CHECK: e -- myIntMap.add(8, 9)
// CHECK: p myIntMap
// CHECK-NEXT: (Map::map(int(64),int(64)
// CHECK-SAME: size = 4
// CHECK-NEXT: [0] = (key = 8, val = 9)
// CHECK-NEXT: [1] = (key = 1, val = 11)
// CHECK-NEXT: [2] = (key = 6, val = 16)
// CHECK-NEXT: [3] = (key = 3, val = 13)
// CHECK: Integer Map Again: {8: 9, 1: 11, 6: 16, 3: 13}

// CHECK: p myRealMap
// CHECK-NEXT: (Map::map(string,real(64)
// CHECK-SAME: size = 3
// CHECK: key = "1.1"
// CHECK: val = 1.
// CHECK: key = "2.2"
// CHECK: val = 2.
// CHECK: key = "3.3"
// CHECK: val = 3.

// CHECK: v myRealMap[1]
// CHECK: key = "2.2"
// CHECK: val = 2.

// CHECK: v myRealMap[1].key
// CHECK: (string) key = "2.2"

// CHECK: v myRealMap[1].val
// CHECK-NEXT: (real(64)) val = 2.

// CHECK: p myRecMap
// CHECK-NEXT: (Map::map(int(64),myRec
// CHECK-SAME: size = 3
// CHECK: key = 1
// CHECK-NEXT: val = (x = 10, y = 20.5)
// CHECK: key = 2
// CHECK-NEXT: val = (x = 30, y = 40.5)
// CHECK: key = 3
// CHECK-NEXT: val = (x = 50, y = 60.5)

// CHECK: v myRecMap[2]
// CHECK: key = 3
// CHECK-NEXT: val = (x = 50, y = 60.5)

// CHECK: v myRecMap[2].val.y
// CHECK-NEXT: {{.*}} = 60.5

// CHECK: p myStrMap
// CHECK-NEXT: (Map::map(string,string
// CHECK-SAME: size = 3
// CHECK: key = "1"
// CHECK: val = "one"
// CHECK: key = "3"
// CHECK: val = "three"
// CHECK: key = "2"
// CHECK: val = "two"

// CHECK: v myStrMap[0]
// CHECK: key = "1"
// CHECK: val = "one"

// CHECK: p myOwnedClassMap
// CHECK-NEXT: (Map::map(string,owned MyClass?
// CHECK-SAME: size = 3
// CHECK: key = "third"
// CHECK: val = 0x{{[0-9a-f]+}}
// CHECK: a = 3
// CHECK: key = "second"
// CHECK: val = 0x{{[0-9a-f]+}}
// CHECK: a = 2
// CHECK: key = "first"
// CHECK: val = 0x{{[0-9a-f]+}}
// CHECK: a = 1

// CHECK: v myOwnedClassMap[2]
// CHECK: key = "first"
// CHECK: val = 0x{{[0-9a-f]+}}
// CHECK: a = 1

// CHECK: v myOwnedClassMap[2].key
// CHECK: (string) key = "first"

// CHECK: v myOwnedClassMap[2].val.a
// CHECK-NEXT: (int(64)) a = 1

// CHECK: p myClassMap
// CHECK-NEXT: (Map::map(int(64),unmanaged MyClass?
// CHECK-SAME: size = 3
// CHECK: key = 1
// CHECK-NEXT: val = 0x{{[0-9a-f]+}}
// CHECK: key = 2
// CHECK-NEXT: val = 0x{{[0-9a-f]+}}
// CHECK: key = 3
// CHECK-NEXT: val = 0x{{[0-9a-f]+}}

// CHECK: v myClassMap[2]
// CHECK: key = 3
// CHECK-NEXT: val = 0x{{[0-9a-f]+}}

// CHECK: v *myClassMap[2].val
// CHECK: a = 3
// CHECK-NEXT: b = 3.
// CHECK: v myClassMap[2].val->b
// CHECK-NEXT: {{.*}} = 3.
