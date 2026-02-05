
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

  // CHECK: hit Breakpoint 1
  // CHECK: at ./basicTypes.chpl:109

  // CHECK-LOCAL: $1 = {myField = [[MYFIELD_ADDR:0x[0-9a-f]+]]}
  // CHECK-NOLOCAL: $1 = {myField = {locale = {node = 0}, addr = [[MYFIELD_ADDR:0x[0-9a-f]+]]}}

  // CHECK-LOCAL: $2 = (struct MyClass *) [[MYFIELD_ADDR]]
  // CHECK-NOLOCAL: $2 = {locale = {node = 0}, addr = [[MYFIELD_ADDR]]}
  var myRec = new MyRecord1();

  // CHECK: $3 = {myFieldA = 0,

  // CHECK: $4 = {x0 = 0, x1 = 0}

  // CHECK: $5 = {buffLen = 0,

  // CHECK-LOCAL: $6 = (struct MyClass *) 0x0
  // CHECK-NOLOCAL: $6 = {locale = {node = 0}, addr = 0x0}

  // CHECK: $7 = {
  // CHECK-LOCAL-SAME: _instance = [[MYARRF_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYARRF_ADDR:0x[0-9a-f]+]]}
  var myRec2 = new MyRecord2();

  // CHECK: $8 = (struct MyClass *) [[MYUNMANAGED_ADDR:0x[0-9a-f]+]]
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }

  // CHECK-LOCAL: $9 = {chpl_p = [[MYCLASS_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL: $9 = {chpl_p = {locale = {node = 0}, addr = [[MYCLASS_ADDR:0x[0-9a-f]+]]}}
  var myClass = new MyClass();

  // CHECK: $10 = {buffLen = 13
  // CHECK: "Hello, world!"
  const myStr = "Hello, world!";

  // CHECK: $11 =
  // CHECK-LOCAL-SAME: _instance = [[MYDOM_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYDOM_ADDR:0x[0-9a-f]+]]}
  const myDom = {1..10, 1..10 by 2};

  // CHECK: $12 =
  // CHECK-LOCAL-SAME: _instance = [[MYARR2D_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYARR2D_ADDR:0x[0-9a-f]+]]}
  const myArr2d: [myDom] int = reshape([i in 1..#myDom.size] i, myDom);

  // CHECK: $13 =
  // CHECK-LOCAL-SAME: _instance = [[MYARR_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYARR_ADDR:0x[0-9a-f]+]]}
  const myArr: [1..10] int = 1..10;

  // CHECK: $14 = true
  const myBool = true;

  // CHECK: $15 = 42
  const myInt: int = 42;

  // CHECK: $16 = 3.14
  const myReal: real = 3.14;

  // CHECK: $17 = e2
  const myEnumVal = myEnum.e2;

  // CHECK: $18 =
  // CHECK-SAME: e2
  const ref myEnumRef = myEnumVal;

  // CHECK: $19 = {x0 = 1,
  const myTuple = (1, "two", 3.0, myEnum.e1);


  writeln(myRec, myRec2, myStr, myUnmanagedClass, myClass, sep=" | ");
  writeln(myDom, myArr, myArr2d, sep=" | ");
  writeln(myBool, myInt, myReal, myEnumVal, myEnumRef, myTuple, sep=" | ");

  use Debugger; breakpoint;
}
