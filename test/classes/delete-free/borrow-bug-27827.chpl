record myRec {
  var x;
}
class MyClass {
  var x: int = 17;
}

proc getBorrow(x: borrowed) do return x;

proc main() {
  var r1 = new myRec(new unmanaged MyClass?());
  defer { delete r1.x; }
  var r2 = new myRec(r1.x.borrow());
  var r3 = new myRec(r1.x:borrowed);
  var r4 = new myRec(getBorrow(r1.x));
  writeln(r1, r2, r3, r4, sep =" | ");
  r4.x!.x = 18;
  writeln(r1, r2, r3, r4, sep =" | ");

  var newUnmanagedClass = new unmanaged MyClass(20);
  defer { delete newUnmanagedClass; }
  r1.x = newUnmanagedClass;
  r2.x = newUnmanagedClass:borrowed;

  writeln(r1, r2, r3, r4, sep =" | ");
}
