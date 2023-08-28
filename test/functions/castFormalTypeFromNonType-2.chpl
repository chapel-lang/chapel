class MyClass {}
type myBorrowedType = borrowed MyClass;

proc bar(x, y: x:myBorrowedType) {
  writeln((x.type:string, y.type:string));
}

var x = new MyClass();
var y = new MyClass();
bar(x, y);
