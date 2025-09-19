use Debugger;


// DWARFDUMP: myRec(MyClass)
// DWARFDUMP: myRec(owned MyClass)
// DWARFDUMP: myRec(shared MyClass)
// DWARFDUMP: myRec(borrowed MyClass)
// DWARFDUMP: myRec(unmanaged MyClass?)
// DWARFDUMP: myRec(owned MyClass?)
// DWARFDUMP: myRec(shared MyClass?)
// DWARFDUMP: myRec(borrowed MyClass?)
// DWARFDUMP: myRec(MyChildClass)
// DWARFDUMP: myRec(owned MyChildClass)
// DWARFDUMP: myRec(shared MyChildClass)
// DWARFDUMP: myRec(borrowed MyChildClass)
// DWARFDUMP: myRec(unmanaged MyChildClass?)
// DWARFDUMP: myRec(owned MyChildClass?)
// DWARFDUMP: myRec(shared MyChildClass?)
// DWARFDUMP: myRec(borrowed MyChildClass?)
record myRec {
  var x;
}
// DWARFDUMP: MyClass
class MyClass {
  var x: int = 17;
}
// DWARFDUMP: MyChildClass
class MyChildClass : MyClass {
  var y: real = 3.14;
}

proc main() {

  // DWARFDUMP: myUnmanagedClass
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }
  // DWARFDUMP: myOwnedClass
  var myOwnedClass = new owned MyClass();
  // DWARFDUMP: mySharedClass
  var mySharedClass = new shared MyClass();
  // DWARFDUMP: myBorrowedClass
  var myBorrowedClass = myUnmanagedClass.borrow();
  // DWARFDUMP: myUnmanagedChildClass
  var myUnmanagedChildClass = new unmanaged MyChildClass();
  defer { delete myUnmanagedChildClass; }
  // DWARFDUMP: myOwnedChildClass
  var myOwnedChildClass = new owned MyChildClass();
  // DWARFDUMP: mySharedChildClass
  var mySharedChildClass = new shared MyChildClass();
  // DWARFDUMP: myBorrowedChildClass
  var myBorrowedChildClass = myUnmanagedChildClass.borrow();

  writeln(myUnmanagedClass, myOwnedClass, mySharedClass, myBorrowedClass, sep =" | ");
  writeln(myUnmanagedChildClass, myOwnedChildClass, mySharedChildClass, myBorrowedChildClass, sep =" | ");
  breakpoint;

  // DWARFDUMP: myNilableUnmanagedClass
  var myNilableUnmanagedClass: unmanaged MyClass? = nil;
  // DWARFDUMP: myNilableOwnedClass
  var myNilableOwnedClass: owned MyClass? = nil;
  // DWARFDUMP: myNilableSharedClass
  var myNilableSharedClass: shared MyClass? = nil;
  // DWARFDUMP: myNilableBorrowedClass
  var myNilableBorrowedClass: borrowed MyClass? = nil;
  // DWARFDUMP: myNilableUnmanagedChildClass
  var myNilableUnmanagedChildClass: unmanaged MyChildClass? = nil;
  // DWARFDUMP: myNilableOwnedChildClass
  var myNilableOwnedChildClass: owned MyChildClass? = nil;
  // DWARFDUMP: myNilableSharedChildClass
  var myNilableSharedChildClass: shared MyChildClass? = nil;
  // DWARFDUMP: myNilableBorrowedChildClass
  var myNilableBorrowedChildClass: borrowed MyChildClass? = nil;

  writeln(myNilableUnmanagedClass, myNilableOwnedClass, myNilableSharedClass, myNilableBorrowedClass, sep =" | ");
  writeln(myNilableUnmanagedChildClass, myNilableOwnedChildClass, myNilableSharedChildClass, myNilableBorrowedChildClass, sep =" | ");
  breakpoint;

  // DWARFDUMP: myRecordWrappedUnmanagedClass
  var myRecordWrappedUnmanagedClass = new myRec(x = new unmanaged MyClass());
  defer { delete myRecordWrappedUnmanagedClass.x; }
  // DWARFDUMP: myRecordWrappedOwnedClass
  var myRecordWrappedOwnedClass = new myRec(x = new owned MyClass());
  // DWARFDUMP: myRecordWrappedSharedClass
  var myRecordWrappedSharedClass = new myRec(x = new shared MyClass());
  // DWARFDUMP: myRecordWrappedBorrowedClass
  var myRecordWrappedBorrowedClass = new myRec(x = myRecordWrappedUnmanagedClass.x.borrow());
  // DWARFDUMP: myRecordWrappedUnmanagedChildClass
  var myRecordWrappedUnmanagedChildClass = new myRec(x = new unmanaged MyChildClass());
  defer { delete myRecordWrappedUnmanagedChildClass.x; }
  // DWARFDUMP: myRecordWrappedOwnedChildClass
  var myRecordWrappedOwnedChildClass = new myRec(x = new owned MyChildClass());
  // DWARFDUMP: myRecordWrappedSharedChildClass
  var myRecordWrappedSharedChildClass = new myRec(x = new shared MyChildClass());
  // DWARFDUMP: myRecordWrappedBorrowedChildClass
  var myRecordWrappedBorrowedChildClass = new myRec(x = myRecordWrappedUnmanagedChildClass.x.borrow());

  writeln(myRecordWrappedUnmanagedClass, myRecordWrappedOwnedClass, myRecordWrappedSharedClass, myRecordWrappedBorrowedClass, sep =" | ");
  writeln(myRecordWrappedUnmanagedChildClass, myRecordWrappedOwnedChildClass, myRecordWrappedSharedChildClass, myRecordWrappedBorrowedChildClass, sep =" | ");
  breakpoint;

  // DWARFDUMP: myNilableRecordWrappedUnmanagedClass
  var myNilableRecordWrappedUnmanagedClass = new myRec(x =  new unmanaged MyClass?());
  defer { delete myNilableRecordWrappedUnmanagedClass.x; }
  // DWARFDUMP: myNilableRecordWrappedOwnedClass
  var myNilableRecordWrappedOwnedClass = new myRec(x = new owned MyClass?());
  // DWARFDUMP: myNilableRecordWrappedSharedClass
  var myNilableRecordWrappedSharedClass = new myRec(x = new shared MyClass?());
  // DWARFDUMP: myNilableRecordWrappedBorrowedClass
  var myNilableRecordWrappedBorrowedClass = new myRec(x = myNilableRecordWrappedUnmanagedClass.x.borrow());
  // DWARFDUMP: myNilableRecordWrappedUnmanagedChildClass
  var myNilableRecordWrappedUnmanagedChildClass = new myRec(x = new unmanaged MyChildClass?());
  defer { delete myNilableRecordWrappedUnmanagedChildClass.x; }
  // DWARFDUMP: myNilableRecordWrappedOwnedChildClass
  var myNilableRecordWrappedOwnedChildClass = new myRec(x = new owned MyChildClass?());
  // DWARFDUMP: myNilableRecordWrappedSharedChildClass
  var myNilableRecordWrappedSharedChildClass = new myRec(x = new shared MyChildClass?());
  // DWARFDUMP: myNilableRecordWrappedBorrowedChildClass
  var myNilableRecordWrappedBorrowedChildClass = new myRec(x = myNilableRecordWrappedUnmanagedChildClass.x.borrow());

  writeln(myNilableRecordWrappedUnmanagedClass, myNilableRecordWrappedOwnedClass, myNilableRecordWrappedSharedClass, myNilableRecordWrappedBorrowedClass, sep =" | ");
  writeln(myNilableRecordWrappedUnmanagedChildClass, myNilableRecordWrappedOwnedChildClass, myNilableRecordWrappedSharedChildClass, myNilableRecordWrappedBorrowedChildClass, sep =" | ");
  breakpoint;

}
