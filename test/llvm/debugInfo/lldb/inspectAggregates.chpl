use List, Set, Map;
use BlockDist, CyclicDist;


record myRecord {
  var x;
  var y: x.type;
}
proc myRecord.compute(s: x.type) {
  return x * s + y;
}
class MyClass {
  var a: int;
  var b: real;
}

proc main() {

  var myIntList = new list([1, 2, 3]);
  var myRecList = new list([new myRecord(1.1, 2.2), new myRecord(3.3, 4.4)]);
  var myStringSet = new set(string, ["a", "b", "c"]);
  var myMap = new map(int, real);
  myMap.add(1, 1.1);
  myMap.replace(1, 2.2);

  var myArr = [i in 1..10] new shared MyClass(i, i*1.0);
  var myBlockDist = blockDist.createArray({1..#10}, owned MyClass?);
  myBlockDist =
    [i in myBlockDist.domain] if i & 1 then new MyClass?(i, i*1.0) else nil;
  var myCycDist = cyclicDist.createArray({1..#10, 1..#10}, int);
  myCycDist = [(i,j) in myCycDist.domain] i*20 + j;

  writeln(myRecList.first.compute(2.0));
  writeln(myRecList);
  writeln(myBlockDist[1], myCycDist[1,1]);

  use Debugger;
  breakpoint;
}


// CHECK: p myIntList
// CHECK-NEXT: (List::list(int(64)
// CHECK-SAME: size = 3
// CHECK-NEXT: [0] = 1
// CHECK-NEXT: [1] = 2
// CHECK-NEXT: [2] = 3

// CHECK: p myRecList
// CHECK-NEXT: (List::list(myRecord(real(64))
// CHECK-SAME: size = 2
// CHECK-NEXT: [0] = (x = 1.{{.*}}, y = 2.{{.*}})
// CHECK-NEXT: [1] = (x = 3.{{.*}}, y = 4.{{.*}})

// CHECK: v myRecList[0]
// CHECK: [0] = (x = 1.{{.*}}, y = 2.{{.*}})

// CHECK: p myRecList._getRef(1)
// CHECK: (x = 3.{{.*}}, y = 4.{{.*}})

// CHECK: p myRecList._getRef(1).x
// CHECK-NEXT: (real(64)) 3.

// CHECK: p myRecList._getRef(1).compute(4.0)
// CHECK-NEXT: (real(64)) 17.6

// CHECK: p myRecList.first()
// CHECK: (x = 1.{{.*}}, y = 2.{{.*}})

// CHECK: p myRecList.first().y
// CHECK-NEXT: (real(64)) 2.

// CHECK: p myRecList.first().compute(3.0)
// CHECK-NEXT: (real(64)) 5.5

// CHECK: p myStringSet
// CHECK-NEXT: (Set::set(string
// CHECK-SAME: size = 3
// CHECK: [0] = "a"
// CHECK: [1] = "c"
// CHECK: [2] = "b"

// CHECK: p myMap
// CHECK-NEXT: (Map::map(int(64),real(64)
// CHECK-SAME: size = 1
// CHECK-NEXT: [0] = (key = 1, val = 2.

// CHECK: e -- myMap.add(2, 17.8)
// CHECK: p myMap
// CHECK-NEXT: (Map::map(int(64),real(64)
// CHECK-SAME: size = 2
// CHECK-NEXT: [0] = (key = 1, val = 2.
// CHECK: [1] = (key = 2, val = 17.

// CHECK: p myArr
// CHECK: [1..10] shared MyClass
// CHECK: [1] = 0x{{[0-9a-f]+}}
// CHECK: a = 1
// CHECK: [2] = 0x{{[0-9a-f]+}}
// CHECK: a = 2
// CHECK: [3] = 0x{{[0-9a-f]+}}
// CHECK: a = 3
// CHECK: [10] = 0x{{[0-9a-f]+}}
// CHECK: a = 10

// CHECK: p myBlockDist
// CHECK: owned MyClass?
// CHECK: [1] = 0x{{[0-9a-f]+}}
// CHECK: a = 1
// CHECK: [2] = nil
// CHECK: [3] = 0x{{[0-9a-f]+}}
// CHECK: a = 3
// CHECK: [4] = nil
// CHECK: [5] = 0x{{[0-9a-f]+}}
// CHECK: a = 5
// CHECK: [6] = nil
// CHECK: [7] = 0x{{[0-9a-f]+}}
// CHECK: a = 7
// CHECK: [8] = nil
// CHECK: [9] = 0x{{[0-9a-f]+}}
// CHECK: a = 9
// CHECK: [10] = nil

// CHECK: p myCycDist
// CHECK: int(64)
// CHECK: [1,1] = 21
// CHECK: [2,1] = 41
// CHECK: [10,1] = 201
// CHECK: [1,10] = 30
// CHECK: [10,10] = 210

