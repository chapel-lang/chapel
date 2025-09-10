
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
  var myRec = new MyRecord1();
  var myRec2 = new MyRecord2();
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }
  var myClass = new MyClass();
  const myStr = "Hello, world!";
  const myDom = {1..10, 1..10 by 2};
  const myArr2d: [myDom] int = reshape([i in 1..#myDom.size] i, myDom);
  const myArr: [1..10] int = 1..10;
  const myBool = true;
  const myInt: int = 42;
  const myReal: real = 3.14;
  const myEnumVal = myEnum.e2;
  const ref myEnumRef = myEnumVal;
  const myTuple = (1, "two", 3.0, myEnum.e1);


  writeln(myRec, myRec2, myStr, myUnmanagedClass, myClass, sep=" | ");
  writeln(myDom, myArr, myArr2d, sep=" | ");
  writeln(myBool, myInt, myReal, myEnumVal, myEnumRef, myTuple, sep=" | ");

  use Debugger; breakpoint;
}
