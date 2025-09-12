use Debugger;
use CTypes;

record myRec {
  var x;
}
class MyClass {
  var x: int = 17;
}

proc main() {

  var myInt = 42;
  var myReal = 3.14;
  var myRecInt = new myRec(x = myInt);
  var myUnmanagedClass = new unmanaged MyClass();
  defer { delete myUnmanagedClass; }
  var myOwnedClass = new owned MyClass();

  {
    var myIntPtr = c_ptrTo(myInt);
    var myRealPtr = c_ptrTo(myReal);
    var myRecIntPtr = c_ptrTo(myRecInt);
    var myUnmanagedClassPtr = c_addrOf(myUnmanagedClass);
    var myOwnedClassPtr = c_addrOf(myOwnedClass);
    writeln(myIntPtr.deref(), myRealPtr.deref(), myRecIntPtr.deref(), myUnmanagedClassPtr.deref(), myOwnedClassPtr.deref(), sep =" | ");
    breakpoint;
  }

  {
    var myIntPtrConst = c_ptrToConst(myInt);
    var myRealPtrConst = c_ptrToConst(myReal);
    var myRecIntPtrConst = c_ptrToConst(myRecInt);
    var myUnmanagedClassPtrConst = c_addrOfConst(myUnmanagedClass);
    var myOwnedClassPtrConst = c_addrOfConst(myOwnedClass);
    writeln(myIntPtrConst.deref(), myRealPtrConst.deref(), myRecIntPtrConst.deref(), myUnmanagedClassPtrConst.deref(), myOwnedClassPtrConst.deref(), sep =" | ");
    breakpoint;
  }


}
