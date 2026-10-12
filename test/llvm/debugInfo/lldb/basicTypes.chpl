
class MyClass {
  var x: int;
  var y: real;
}

record MyRecord1 {
  var myField: unmanaged MyClass;
  proc init() {
    this.myField = new unmanaged MyClass();
  }
  proc deinit() {
    delete this.myField;
  }
}

record MyRecord2 {
  var myFieldA: int;
  var myFieldB: real;
  var myFieldC: (int, real);
  var myFieldD: string;
  var myFieldE: unmanaged MyClass?;
  var myFieldF: [1..10] int;
}

enum myEnum {
  e1,
  e2,
  e3
}

proc main() {

  // CHECK: stop reason = breakpoint
  // CHECK-NEXT: basicTypes.chpl:244

  // CHECK: p myRec
  // CHECK-NEXT: (basicTypes::MyRecord1) {
  // CHECK-LOCAL-NEXT: myField = [[MYFIELD_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-NEXT: myField = {
  // CHECK-NOLOCAL-NEXT: locale = (node = 0)
  // CHECK-NOLOCAL-NEXT: addr = [[MYFIELD_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-NEXT: }
  // CHECK-NEXT: }

  // CHECK: p myRec.myField
  // CHECK-LOCAL-NEXT: (basicTypes::MyClass *) [[MYFIELD_ADDR]]
  // CHECK-NOLOCAL-NEXT: (wide(MyClass)) {
  // CHECK-NOLOCAL-NEXT: locale = (node = 0)
  // CHECK-NOLOCAL-NEXT: addr = [[MYFIELD_ADDR]]
  // CHECK-NOLOCAL-NEXT: }
  var myRec = new MyRecord1();

  // CHECK: p myRec2
  // CHECK-NEXT: basicTypes::MyRecord2
  // CHECK-NEXT: myFieldA = 0
  // CHECK-NEXT: myFieldB = 0
  // CHECK-NEXT: myFieldC = (x0 = 0, x1 = 0)
  // CHECK-NEXT: myFieldD = ""
  // CHECK-LOCAL: myFieldE = nullptr
  // CHECK-NOLOCAL: myFieldE = {
  // CHECK-NOLOCAL-NEXT: locale = (node = 0)
  // CHECK-NOLOCAL-NEXT: addr = nullptr
  // CHECK-NOLOCAL-NEXT: }
  // CHECK-NEXT: myFieldF = [1..10] int(64)
  // CHECK-NEXT: [1] = 0
  // CHECK-NEXT: [2] = 0
  // CHECK-NEXT: [3] = 0
  // CHECK-NEXT: [4] = 0
  // CHECK-NEXT: [5] = 0
  // CHECK-NEXT: [6] = 0
  // CHECK-NEXT: [7] = 0
  // CHECK-NEXT: [8] = 0
  // CHECK-NEXT: [9] = 0
  // CHECK-NEXT: [10] = 0

  // CHECK: p myRec2.myFieldC
  // CHECK-NEXT: ((int(64),real(64)))  (x0 = 0, x1 = 0)
  // CHECK: p myRec2.myFieldD
  // CHECK-NEXT: (string) ""

  // CHECK: p myRec2.myFieldE
  // CHECK-LOCAL-NEXT: (basicTypes::MyClass *) nullptr
  // CHECK-NOLOCAL-NEXT: (wide(MyClass)) {
  // CHECK-NOLOCAL-NEXT: locale = (node = 0)
  // CHECK-NOLOCAL-NEXT: addr = nullptr
  // CHECK-NOLOCAL-NEXT: }

  // CHECK: p myRec2.myFieldF
  // CHECK-NEXT: ([domain(1,int(64),one)] int(64)) [1..10] int(64)
  // CHECK-NEXT: [1] = 0
  // CHECK-NEXT: [2] = 0
  // CHECK-NEXT: [3] = 0
  // CHECK-NEXT: [4] = 0
  // CHECK-NEXT: [5] = 0
  // CHECK-NEXT: [6] = 0
  // CHECK-NEXT: [7] = 0
  // CHECK-NEXT: [8] = 0
  // CHECK-NEXT: [9] = 0
  // CHECK-NEXT: [10] = 0
  var myRec2 = new MyRecord2();

  // CHECK: p myUnmanagedClass
  // CHECK-NEXT: (basicTypes::MyClass *) 0x{{[0-9a-f]+}}
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }

  // CHECK: p myClass
  // CHECK-NEXT: (owned MyClass) 0x{{[0-9a-f]+}} {
  // CHECK-NEXT: super = (cid = {{[0-9]+}})
  // CHECK-NEXT: x = 0
  // CHECK-NEXT: y = 0
  // CHECK-NEXT: }
  var myClass = new MyClass();

  // CHECK: p myStr
  // CHECK-NEXT: (string) "Hello, world!"
  // CHECK-NEXT: size = 13
  const myStr = "Hello, world!";

  // CHECK: p myDom
  // CHECK-NEXT: (domain(2,int(64),positive)) {1..10 by 1, 1..10 by 2} {
  // CHECK-NEXT: dim = {
  // CHECK-NEXT: [0] = 1..10 by 1 {
  // CHECK-NEXT: low = 1
  // CHECK-NEXT: high = 10
  // CHECK-NEXT: stride = 1
  // CHECK-NEXT: }
  // CHECK-NEXT: [1] = 1..10 by 2 {
  // CHECK-NEXT: low = 1
  // CHECK-NEXT: high = 10
  // CHECK-NEXT: stride = 2
  // CHECK-NEXT: }
  // CHECK-NEXT: }
  // CHECK-NEXT: }
  const myDom = {1..10, 1..10 by 2};

  // CHECK: p myArr2d
  // CHECK-NEXT: ([domain(2,int(64),positive)] int(64)) [1..10 by 1, 1..10 by 2] int(64) {
  // CHECK-NEXT: [1,1] = 1
  // CHECK-NEXT: [2,1] = 6
  // CHECK-NEXT: [3,1] = 11
  // CHECK-NEXT: [4,1] = 16
  // CHECK-NEXT: [5,1] = 21
  // CHECK-NEXT: [6,1] = 26
  // CHECK-NEXT: [7,1] = 31
  // CHECK-NEXT: [8,1] = 36
  // CHECK-NEXT: [9,1] = 41
  // CHECK-NEXT: [10,1] = 46
  // CHECK-NEXT: [1,3] = 2
  // CHECK-NEXT: [2,3] = 7
  // CHECK-NEXT: [3,3] = 12
  // CHECK-NEXT: [4,3] = 17
  // CHECK-NEXT: [5,3] = 22
  // CHECK-NEXT: [6,3] = 27
  // CHECK-NEXT: [7,3] = 32
  // CHECK-NEXT: [8,3] = 37
  // CHECK-NEXT: [9,3] = 42
  // CHECK-NEXT: [10,3] = 47
  // CHECK-NEXT: [1,5] = 3
  // CHECK-NEXT: [2,5] = 8
  // CHECK-NEXT: [3,5] = 13
  // CHECK-NEXT: [4,5] = 18
  // CHECK-NEXT: [5,5] = 23
  // CHECK-NEXT: [6,5] = 28
  // CHECK-NEXT: [7,5] = 33
  // CHECK-NEXT: [8,5] = 38
  // CHECK-NEXT: [9,5] = 43
  // CHECK-NEXT: [10,5] = 48
  // CHECK-NEXT: [1,7] = 4
  // CHECK-NEXT: [2,7] = 9
  // CHECK-NEXT: [3,7] = 14
  // CHECK-NEXT: [4,7] = 19
  // CHECK-NEXT: [5,7] = 24
  // CHECK-NEXT: [6,7] = 29
  // CHECK-NEXT: [7,7] = 34
  // CHECK-NEXT: [8,7] = 39
  // CHECK-NEXT: [9,7] = 44
  // CHECK-NEXT: [10,7] = 49
  // CHECK-NEXT: [1,9] = 5
  // CHECK-NEXT: [2,9] = 10
  // CHECK-NEXT: [3,9] = 15
  // CHECK-NEXT: [4,9] = 20
  // CHECK-NEXT: [5,9] = 25
  // CHECK-NEXT: [6,9] = 30
  // CHECK-NEXT: [7,9] = 35
  // CHECK-NEXT: [8,9] = 40
  // CHECK-NEXT: [9,9] = 45
  // CHECK-NEXT: [10,9] = 50
  // CHECK-NEXT: }
  const myArr2d: [myDom] int = reshape([i in 1..#myDom.size] i, myDom);

  // CHECK: p myArr
  // CHECK-NEXT: ([domain(1,int(64),one)] int(64)) [1..10] int(64) {
  // CHECK-NEXT: [1] = 1
  // CHECK-NEXT: [2] = 2
  // CHECK-NEXT: [3] = 3
  // CHECK-NEXT: [4] = 4
  // CHECK-NEXT: [5] = 5
  // CHECK-NEXT: [6] = 6
  // CHECK-NEXT: [7] = 7
  // CHECK-NEXT: [8] = 8
  // CHECK-NEXT: [9] = 9
  // CHECK-NEXT: [10] = 10
  // CHECK-NEXT: }
  const myArr: [1..10] int = 1..10;

  // CHECK: p myBool
  // CHECK-NEXT: (bool) true
  const myBool = true;

  // CHECK: p myInt
  // CHECK-NEXT: (int(64)) 42
  const myInt: int = 42;

  // CHECK: p myReal
  // CHECK-NEXT: (real(64)) 3.14
  const myReal: real = 3.14;

  // CHECK: p myEnumVal
  // CHECK-NEXT: (basicTypes::myEnum) e2
  const myEnumVal = myEnum.e2;

  // CHECK: p myEnumRef
  // CHECK-NEXT: (_ref(myEnum)) 0x{{[0-9a-f]+}} (&myEnumRef = e2)
  const ref myEnumRef = myEnumVal;

  // CHECK: p myTuple
  // CHECK-NEXT: ((int(64),string,real(64),myEnum)) {
  // CHECK-NEXT: x0 = 1
  // CHECK-NEXT: x1 = "two" {
  // CHECK-NEXT: size = 3
  // CHECK-NEXT: }
  // CHECK-NEXT: x2 = 3
  // CHECK-NEXT: x3 = e1
  // CHECK-NEXT: }
  const myTuple = (1, "two", 3.0, myEnum.e1);


  writeln(myRec, myRec2, myStr, myUnmanagedClass, myClass, sep=" | ");
  writeln(myDom, myArr, myArr2d, sep=" | ");
  writeln(myBool, myInt, myReal, myEnumVal, myEnumRef, myTuple, sep=" | ");

  use Debugger; breakpoint;
}
