use Debugger;


record myRec {
  var x;
}
class MyClass {
  var x: int = 17;
}

proc main() {

  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }
  var myOwnedClass = new owned MyClass();
  var mySharedClass = new shared MyClass();
  var myBorrowedClass = myUnmanagedClass.borrow();

  writeln(myUnmanagedClass, myOwnedClass, mySharedClass, myBorrowedClass, sep =" | ");
  breakpoint;

  var myNilableUnmanagedClass: unmanaged MyClass? = nil;
  var myNilableOwnedClass: owned MyClass? = nil;
  var myNilableSharedClass: shared MyClass? = nil;
  var myNilableBorrowedClass: borrowed MyClass? = nil;

  writeln(myNilableUnmanagedClass, myNilableOwnedClass, myNilableSharedClass, myNilableBorrowedClass, sep =" | ");
  breakpoint;

  var myRecordWrappedUnmanagedClass = new myRec(x = new unmanaged MyClass());
  defer { delete myRecordWrappedUnmanagedClass.x; }
  var myRecordWrappedOwnedClass = new myRec(x = new owned MyClass());
  var myRecordWrappedSharedClass = new myRec(x = new shared MyClass());
  var myRecordWrappedBorrowedClass = new myRec(x = myRecordWrappedUnmanagedClass.x.borrow());

  writeln(myRecordWrappedUnmanagedClass, myRecordWrappedOwnedClass, myRecordWrappedSharedClass, myRecordWrappedBorrowedClass, sep =" | ");
  breakpoint;

  var myNilableRecordWrappedUnmanagedClass = new myRec(x =  new unmanaged MyClass?());
  defer { delete myNilableRecordWrappedUnmanagedClass.x; }
  var myNilableRecordWrappedOwnedClass = new myRec(x = new owned MyClass?());
  var myNilableRecordWrappedSharedClass = new myRec(x = new shared MyClass?());
  var myNilableRecordWrappedBorrowedClass = new myRec(x = myNilableRecordWrappedUnmanagedClass.x.borrow());

  writeln(myNilableRecordWrappedUnmanagedClass, myNilableRecordWrappedOwnedClass, myNilableRecordWrappedSharedClass, myNilableRecordWrappedBorrowedClass, sep =" | ");
  breakpoint;

}
