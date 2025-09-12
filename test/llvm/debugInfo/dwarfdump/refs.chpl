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
    // DWARFDUMP: myIntRef
    ref myIntRef = myInt;
    // DWARFDUMP: myRealRef
    ref myRealRef = myReal;
    // DWARFDUMP: myRecIntRef
    ref myRecIntRef = myRecInt;
    // DWARFDUMP: myUnmanagedClassRef
    ref myUnmanagedClassRef = myUnmanagedClass;
    // DWARFDUMP: myOwnedClassRef
    ref myOwnedClassRef = myOwnedClass;
    writeln(myIntRef, myRealRef, myRecIntRef, myUnmanagedClassRef, myOwnedClassRef, sep =" | ");
    breakpoint;
  }

  {
    // DWARFDUMP: myIntCRef
    const ref myIntCRef = myInt;
    // DWARFDUMP: myRealCRef
    const ref myRealCRef = myReal;
    // DWARFDUMP: myRecIntCRef
    const ref myRecIntCRef = myRecInt;
    // DWARFDUMP: myUnmanagedClassCRef
    const ref myUnmanagedClassCRef = myUnmanagedClass;
    // DWARFDUMP: myOwnedClassCRef
    const ref myOwnedClassCRef = myOwnedClass;
    writeln(myIntCRef, myRealCRef, myRecIntCRef, myUnmanagedClassCRef, myOwnedClassCRef, sep =" | ");
    breakpoint;
  }


}
