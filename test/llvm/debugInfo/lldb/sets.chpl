use Set;
use Debugger;

record myRec {
  var x: int;
  var y: real;
}
class MyClass {
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

  var myOwnedClassSet = createSet(set(owned MyClass?, true),
                              new MyClass(1, 1.1),
                              new MyClass(2, 2.2),
                              new MyClass(3, 3.3));
  writeln("Class Set: ", myOwnedClassSet);
  breakpoint;
  writeln("Class Set Again: ", myOwnedClassSet);


  var myClassSet = createSet(set(unmanaged MyClass?),
                              new unmanaged MyClass(1, 1.1),
                              new unmanaged MyClass(2, 2.2),
                              new unmanaged MyClass(3, 3.3));
  writeln("Class Set: ", myClassSet);
  breakpoint;
  writeln("Class Set Again: ", myClassSet);
  for x in myClassSet do delete x;
}


// CHECK: p myIntSet
// CHECK-NEXT: (Set::set(int(64)
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = 1
// CHECK-NEXT: [1] = 2
// CHECK-NEXT: [2] = 3

// CHECK: v myIntSet[1]
// CHECK-NEXT: (int(64)) {{.*}} = 2

// CHECK: e -- myIntSet.add(10)
// CHECK: p myIntSet
// CHECK-NEXT: (Set::set(int(64)
// CHECK-SAME: size = 4
// CHECK-NEXT: [0] = 1
// CHECK-NEXT: [1] = 2
// CHECK-NEXT: [2] = 10
// CHECK-NEXT: [3] = 3
// CHECK: Integer Set Again: {1, 2, 10, 3}

// CHECK: p myRealSet
// CHECK-NEXT: (Set::set(real(64)
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = 2.
// CHECK-NEXT: [1] = 1.
// CHECK-NEXT: [2] = 3.

// CHECK: v myRealSet[1]
// CHECK-NEXT: (real(64)) {{.*}} = 1.

// CHECK: e -- myRealSet.add(8.8)
// CHECK: p myRealSet
// CHECK-NEXT: (Set::set(real(64)
// CHECK-SAME: size = 4
// CHECK-NEXT: [0] = 2.
// CHECK-NEXT: [1] = 1.
// CHECK-NEXT: [2] = 8.
// CHECK-NEXT: [3] = 3.
// CHECK: Real Set Again: {2.2, 1.1, 8.8, 3.3}

// CHECK: p myRecSet
// CHECK-NEXT: (Set::set(myRec
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = (x = 30, y = 40.5)
// CHECK-NEXT: [1] = (x = 50, y = 60.5)
// CHECK-NEXT: [2] = (x = 10, y = 20.5)

// CHECK: v myRecSet[2]
// CHECK-NEXT: {{.*}} = (x = 10, y = 20.5)

// CHECK: v myRecSet[2].y
// CHECK-NEXT: 20.5

// CHECK: p myStrSet
// CHECK-NEXT: (Set::set(string
// CHECK-SAME: size = 3
// CHECK: [0] = "one"
// CHECK: [1] = "two"
// CHECK: [2] = "three"

// CHECK: v myStrSet[0]
// CHECK-NEXT: (string) {{.*}} = "one"

// CHECK: p myOwnedClassSet
// CHECK-NEXT: (Set::set(owned MyClass?
// CHECK-SAME: size = 3
// CHECK: [0] = 0x{{[0-9a-f]+}}
// CHECK: [1] = 0x{{[0-9a-f]+}}
// CHECK: [2] = 0x[[OWNED_SET_2_ADDR:[0-9a-f]+]]

// CHECK: v myOwnedClassSet[2]
// CHECK-NEXT: (owned MyClass?) {{.*}} = 0x[[OWNED_SET_2_ADDR]]

// CHECK: v myOwnedClassSet[2].b
// CHECK-NEXT: b =

// CHECK: p myClassSet
// CHECK-NEXT: (Set::set(unmanaged MyClass?
// CHECK-SAME: size = 3
// CHECK: [0] = 0x{{[0-9a-f]+}}
// CHECK: [1] = 0x{{[0-9a-f]+}}
// CHECK: [2] = 0x{{[0-9a-f]+}}

// CHECK: v myClassSet[2]
// CHECK-NEXT: {{.*}} = 0x{{[0-9a-f]+}}
// CHECK: v *myClassSet[2]
// CHECK: a = [[CLASS_SET_2_A:[0-9.]+]]
// CHECK-NEXT: b = [[CLASS_SET_2_B:[0-9.]+]]
// CHECK: v myClassSet[2]->b
// CHECK-NEXT: {{.*}} = [[CLASS_SET_2_B]]
