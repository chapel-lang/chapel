
// DWARFDUMP: MyClass
class MyClass {
  var x: int;
  var y: real;
}

// DWARFDUMP: MyRecord1
record MyRecord1 {
  var myField: unmanaged MyClass;
  proc init() {
    this.myField = new unmanaged MyClass();
  }
  proc deinit() {
    delete this.myField;
  }
}

// DWARFDUMP: MyRecord2
record MyRecord2 {
  var myFieldA: int;
  var myFieldB: real;
  var myFieldC: (int, real);
  var myFieldD: string;
  var myFieldE: unmanaged MyClass?;
  var myFieldF: [1..10] int;
}

// DWARFDUMP: myEnum
enum myEnum {
  e1,
  e2,
  e3
}

proc main() {
  // DWARFDUMP: myRec
  var myRec = new MyRecord1();
  // DWARFDUMP: myRec2
  var myRec2 = new MyRecord2();
  // DWARFDUMP: myUnmanagedClass
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }
  // DWARFDUMP: myClass
  var myClass = new MyClass();
  // DWARFDUMP: myStr
  const myStr = "Hello, world!";
  // DWARFDUMP: myDom
  const myDom = {1..10, 1..10 by 2};
  // DWARFDUMP: myArr2d
  const myArr2d: [myDom] int = reshape([i in 1..#myDom.size] i, myDom);
  // DWARFDUMP: myArr
  const myArr: [1..10] int = 1..10;
  // DWARFDUMP: myBool
  const myBool = true;
  // DWARFDUMP: myInt
  const myInt: int = 42;
  // DWARFDUMP: myReal
  const myReal: real = 3.14;
  // DWARFDUMP: myEnumVal
  const myEnumVal = myEnum.e2;
  // DWARFDUMP: myEnumRef
  const ref myEnumRef = myEnumVal;
  // DWARFDUMP: myTuple
  const myTuple = (1, "two", 3.0, myEnum.e1);

  // DWARFDUMP: [domain(1,int(64),one)] int(64)
  // DWARFDUMP: domain(2,int(64),positive)

  writeln(myRec, myRec2, myStr, myUnmanagedClass, myClass, sep=" | ");
  writeln(myDom, myArr, myArr2d, sep=" | ");
  writeln(myBool, myInt, myReal, myEnumVal, myEnumRef, myTuple, sep=" | ");

  use Debugger; breakpoint;
}
