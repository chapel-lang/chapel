use Debugger;
use CTypes;

// DWARFDUMP: myRec(int(64))
record myRec {
  var x;
}
// DWARFDUMP: MyClass
class MyClass {
  var x: int = 17;
}

proc main() {

  // DWARFDUMP: myInt
  var myInt = 42;
  // DWARFDUMP: myReal
  var myReal = 3.14;
  // DWARFDUMP: myRecInt
  var myRecInt = new myRec(x = myInt);
  // DWARFDUMP: myUnmanagedClass
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }
  // DWARFDUMP: myOwnedClass
  var myOwnedClass = new owned MyClass();

  {
    // DWARFDUMP: myIntPtr
    var myIntPtr = c_ptrTo(myInt);
    // DWARFDUMP: myRealPtr
    var myRealPtr = c_ptrTo(myReal);
    // DWARFDUMP: myRecIntPtr
    var myRecIntPtr = c_ptrTo(myRecInt);
    // DWARFDUMP: myUnmanagedClassPtr
    var myUnmanagedClassPtr = c_addrOf(myUnmanagedClass);
    // DWARFDUMP: myOwnedClassPtr
    var myOwnedClassPtr = c_addrOf(myOwnedClass);

    writeln(myIntPtr.deref(), myRealPtr.deref(), myRecIntPtr.deref(), myUnmanagedClassPtr.deref(), myOwnedClassPtr.deref(), sep =" | ");
    breakpoint;
  }

  {
    // DWARFDUMP: myIntPtrConst
    var myIntPtrConst = c_ptrToConst(myInt);
    // DWARFDUMP: myRealPtrConst
    var myRealPtrConst = c_ptrToConst(myReal);
    // DWARFDUMP: myRecIntPtrConst
    var myRecIntPtrConst = c_ptrToConst(myRecInt);
    // DWARFDUMP: myUnmanagedClassPtrConst
    var myUnmanagedClassPtrConst = c_addrOfConst(myUnmanagedClass);
    // DWARFDUMP: myOwnedClassPtrConst
    var myOwnedClassPtrConst = c_addrOfConst(myOwnedClass);

    writeln(myIntPtrConst.deref(), myRealPtrConst.deref(), myRecIntPtrConst.deref(), myUnmanagedClassPtrConst.deref(), myOwnedClassPtrConst.deref(), sep =" | ");
    breakpoint;
  }


}
