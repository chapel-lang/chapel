use List;
use Debugger;

record myRec {
  var x: int;
  var y: real;
}
class MyClass {
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


  var myOwnedClassList = createList(list(owned MyClass?, true),
                              new MyClass(1, 1.1),
                              nil,
                              new MyClass(3, 3.3));
  writeln("Class List: ", myOwnedClassList);
  breakpoint;
  writeln("Class List Again: ", myOwnedClassList);


  var myClassList = createList(list(unmanaged MyClass?),
                              new unmanaged MyClass(1, 1.1),
                              nil,
                              new unmanaged MyClass(3, 3.3));
  writeln("Class List: ", myClassList);
  breakpoint;
  writeln("Class List Again: ", myClassList);
  for x in myClassList do delete x;
}


// CHECK: p myIntList
// CHECK-NEXT: (List::list(int(64)
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = 1
// CHECK-NEXT: [1] = 2
// CHECK-NEXT: [2] = 3

// CHECK: v myIntList[1]
// CHECK-NEXT: (int(64)) [1] = 2

// CHECK: e -- myIntList.pushBack(8)
// CHECK: p myIntList
// CHECK-NEXT: (List::list(int(64)
// CHECK-SAME: size = 4
// CHECK-NEXT: [0] = 1
// CHECK-NEXT: [1] = 2
// CHECK-NEXT: [2] = 3
// CHECK-NEXT: [3] = 8
// CHECK: Integer List Again: [1, 2, 3, 8]

// CHECK: p myRealList
// CHECK-NEXT: (List::list(real(64)
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = 1.
// CHECK-NEXT: [1] = 2.
// CHECK-NEXT: [2] = 3.

// CHECK: v myRealList[1]
// CHECK-NEXT: (real(64)) [1] = 2.

// CHECK: e -- myRealList.pushBack(8.8)
// CHECK: p myRealList
// CHECK-NEXT: (List::list(real(64)
// CHECK-SAME: size = 4
// CHECK-NEXT: [0] = 1.
// CHECK-NEXT: [1] = 2.
// CHECK-NEXT: [2] = 3.
// CHECK-NEXT: [3] = 8.
// CHECK: Real List Again: [1.1, 2.2, 3.3, 8.8]

// CHECK: p myRecList
// CHECK-NEXT: (List::list(myRec
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = (x = 10, y = 20.5)
// CHECK-NEXT: [1] = (x = 30, y = 40.5)
// CHECK-NEXT: [2] = (x = 50, y = 60.5)

// CHECK: v myRecList[2]
// CHECK-NEXT: [2] = (x = 50, y = 60.5)

// CHECK: v myRecList[2].y
// CHECK-NEXT: 60.5

// CHECK: p myStrList
// CHECK-NEXT: (List::list(string
// CHECK-SAME: size = 3
// CHECK: [0] = "one"
// CHECK: [1] = "two"
// CHECK: [2] = "three"

// CHECK: v myStrList[0]
// CHECK-NEXT: (string) [0] = "one"


// CHECK: p myOwnedClassList
// CHECK-NEXT: (List::list(owned MyClass?
// CHECK-SAME: size = 3
// CHECK: [0] = 0x{{[0-9a-f]+}}
// CHECK: [1] = nil
// CHECK: [2] = 0x[[OWNED_CLASS_2_ADDR:[0-9a-f]+]]

// CHECK: v myOwnedClassList[2]
// CHECK-NEXT: (owned MyClass?) [2] = 0x[[OWNED_CLASS_2_ADDR]]

// CHECK: v myOwnedClassList[2].b
// CHECK-NEXT: b = 3.

// CHECK: p myClassList
// CHECK-NEXT: (List::list(unmanaged MyClass?
// CHECK-SAME: size = 3
// CHECK: [0] = 0x{{[0-9a-f]+}}
// CHECK: [1] = n
// CHECK: [2] = 0x{{[0-9a-f]+}}

// CHECK: v myClassList[2]
// CHECK-NEXT: [2] = 0x{{[0-9a-f]+}}
// CHECK: v myClassList[2]->b
// CHECK-NEXT: b = 3.

