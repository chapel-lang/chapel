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
    ref myIntRef = myInt;
    ref myRealRef = myReal;
    ref myRecIntRef = myRecInt;
    ref myUnmanagedClassRef = myUnmanagedClass;
    ref myOwnedClassRef = myOwnedClass;
    writeln(myIntRef, myRealRef, myRecIntRef, myUnmanagedClassRef, myOwnedClassRef, sep =" | ");
    breakpoint;
  }

  {
    const ref myIntCRef = myInt;
    const ref myRealCRef = myReal;
    const ref myRecIntCRef = myRecInt;
    const ref myUnmanagedClassCRef = myUnmanagedClass;
    const ref myOwnedClassCRef = myOwnedClass;
    writeln(myIntCRef, myRealCRef, myRecIntCRef, myUnmanagedClassCRef, myOwnedClassCRef, sep =" | ");
    breakpoint;
  }


}
