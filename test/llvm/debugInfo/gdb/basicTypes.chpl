
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
  // CHECK: at ./basicTypes.chpl:184

  // CHECK-LOCAL: $1 = {myField = [[MYFIELD_ADDR:0x[0-9a-f]+]]}
  // CHECK-NOLOCAL: $1 = {myField = {locale = {node = 0}, addr = [[MYFIELD_ADDR:0x[0-9a-f]+]]}}
  // CHECK: type = struct MyRecord1 {
  // CHECK-LOCAL-NEXT: struct MyClass *myField;
  // CHECK-NOLOCAL-NEXT: struct wide(MyClass) myField;
  // CHECK-NEXT: }

  // CHECK-LOCAL: $2 = (struct MyClass *) [[MYFIELD_ADDR]]
  // CHECK-LOCAL-NEXT: type = struct MyClass {
  // CHECK-LOCAL-NEXT: struct RootClass super
  // CHECK-LOCAL-NEXT: int(64) x;
  // CHECK-LOCAL-NEXT: real(64) y;
  // CHECK-LOCAL-NEXT: } *
  // CHECK-NOLOCAL: $2 = {locale = {node = 0}, addr = [[MYFIELD_ADDR]]}
  // CHECK-NOLOCAL-NEXT: type = struct wide(MyClass) {
  // CHECK-NOLOCAL-NEXT: struct chpl_localeID_t locale;
  // CHECK-NOLOCAL-NEXT: struct MyClass *addr;
  // CHECK-NOLOCAL-NEXT: }
  var myRec = new MyRecord1();

  // CHECK: $3 = {myFieldA = 0,
  // CHECK: type = struct MyRecord2 {
  // CHECK-NEXT: int(64) myFieldA;
  // CHECK-NEXT: real(64) myFieldB;
  // CHECK-NEXT: struct (int(64),real(64)) myFieldC;
  // CHECK-NEXT: struct string myFieldD;
  // CHECK-LOCAL-NEXT: struct MyClass *myFieldE;
  // CHECK-NOLOCAL-NEXT: struct wide(MyClass) myFieldE;
  // CHECK-NEXT: struct [domain(1,int(64),one)] int(64) myFieldF;

  // CHECK: $4 = {x0 = 0, x1 = 0}
  // CHECK-NEXT: type = struct (int(64),real(64)) {
  // CHECK-NEXT: int(64) x0;
  // CHECK-NEXT: real(64) x1;
  // CHECK-NEXT: }

  // CHECK: $5 = {buffLen = 0,
  // CHECK: type = struct string {
  // CHECK-NEXT: int(64) buffLen;

  // CHECK-LOCAL: $6 = (struct MyClass *) 0x0
  // CHECK-LOCAL-NEXT: type = struct MyClass {
  // CHECK-LOCAL-NEXT: struct RootClass super
  // CHECK-LOCAL-NEXT: int(64) x;
  // CHECK-LOCAL-NEXT: real(64) y;
  // CHECK-LOCAL-NEXT: } *
  // CHECK-NOLOCAL: $6 = {locale = {node = 0}, addr = 0x0}
  // CHECK-NOLOCAL-NEXT: type = struct wide(MyClass) {
  // CHECK-NOLOCAL-NEXT: struct chpl_localeID_t locale;
  // CHECK-NOLOCAL-NEXT: struct MyClass *addr;
  // CHECK-NOLOCAL-NEXT: }

  // CHECK: $7 = {
  // CHECK-LOCAL-SAME: _instance = [[MYARRF_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYARRF_ADDR:0x[0-9a-f]+]]}
  // CHECK: type = struct [domain(1,int(64),one)] int(64) {
  // CHECK-LOCAL: struct [domain(1,int(64),one)] int(64) *_instance;
  // CHECK-NOLOCAL: struct wide([domain(1,int(64),one)] int(64)) _instance;
  var myRec2 = new MyRecord2();

  // CHECK: $8 = (struct MyClass *) [[MYUNMANAGED_ADDR:0x[0-9a-f]+]]
  // CHECK-NEXT: type = struct MyClass {
  // CHECK-NEXT: struct RootClass super
  // CHECK-NEXT: int(64) x;
  // CHECK-NEXT: real(64) y;
  // CHECK-NEXT: } *
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }

  // CHECK-LOCAL: $9 = {chpl_p = [[MYCLASS_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL: $9 = {chpl_p = {locale = {node = 0}, addr = [[MYCLASS_ADDR:0x[0-9a-f]+]]}}
  // CHECK: type = struct owned MyClass {
  // CHECK-LOCAL-NEXT: struct MyClass *chpl_p;
  // CHECK-NOLOCAL-NEXT: struct wide(MyClass) chpl_p;
  // CHECK-NEXT: }
  var myClass = new MyClass();

  // CHECK: $10 = {buffLen = 13
  // CHECK: "Hello, world!"
  // CHECK: type = struct string {
  // CHECK-NEXT: int(64) buffLen;
  const myStr = "Hello, world!";

  // CHECK: $11 =
  // CHECK-LOCAL-SAME: _instance = [[MYDOM_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYDOM_ADDR:0x[0-9a-f]+]]}
  // CHECK: type = struct domain(2,int(64),positive) {
  // CHECK-LOCAL: struct domain(2,int(64),positive) *_instance;
  // CHECK-NOLOCAL: struct wide(domain(2,int(64),positive)) _instance;
  const myDom = {1..10, 1..10 by 2};

  // CHECK: $12 =
  // CHECK-LOCAL-SAME: _instance = [[MYARR2D_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYARR2D_ADDR:0x[0-9a-f]+]]}
  // CHECK: type = struct [domain(2,int(64),positive)] int(64) {
  // CHECK-LOCAL: struct [domain(2,int(64),positive)] int(64) *_instance;
  // CHECK-NOLOCAL: struct wide([domain(2,int(64),positive)] int(64)) _instance;
  const myArr2d: [myDom] int = reshape([i in 1..#myDom.size] i, myDom);

  // CHECK: $13 =
  // CHECK-LOCAL-SAME: _instance = [[MYARR_ADDR:0x[0-9a-f]+]]
  // CHECK-NOLOCAL-SAME: _instance = {locale = {node = 0}, addr = [[MYARR_ADDR:0x[0-9a-f]+]]}
  // CHECK: type = struct [domain(1,int(64),one)] int(64) {
  // CHECK-LOCAL: struct [domain(1,int(64),one)] int(64) *_instance;
  // CHECK-NOLOCAL: struct wide([domain(1,int(64),one)] int(64)) _instance;
  const myArr: [1..10] int = 1..10;

  // CHECK: $14 = true
  // CHECK-NEXT: type = bool
  const myBool = true;

  // CHECK: $15 = 42
  // CHECK-NEXT: type = int(64)
  const myInt: int = 42;

  // CHECK: $16 = 3.14
  // CHECK-NEXT: type = real(64)
  const myReal: real = 3.14;

  // CHECK: $17 = e2
  // CHECK-NEXT: type = enum
  // CHECK-SAME: myEnum
  // CHECK-SAME: e2
  const myEnumVal = myEnum.e2;

  // CHECK: $18 =
  // CHECK-SAME: e2
  // CHECK-NEXT: type = enum
  // CHECK-SAME: myEnum
  // CHECK-SAME: e2
  // CHECK-SAME: &
  const ref myEnumRef = myEnumVal;

  // CHECK: $19 = {x0 = 1,
  // CHECK: type = struct (int(64),string,real(64),myEnum) {
  // CHECK-NEXT: int(64) x0;
  // CHECK-NEXT: struct string x1;
  // CHECK-NEXT: real(64) x2;
  // CHECK-NEXT: enum myEnum x3;
  // CHECK-NEXT: }
  const myTuple = (1, "two", 3.0, myEnum.e1);


  writeln(myRec, myRec2, myStr, myUnmanagedClass, myClass, sep=" | ");
  writeln(myDom, myArr, myArr2d, sep=" | ");
  writeln(myBool, myInt, myReal, myEnumVal, myEnumRef, myTuple, sep=" | ");

  use Debugger; breakpoint;
}
