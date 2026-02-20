// Test for issue #28008
// c_addrOf on unmanaged class should work with wide pointers

use CTypes;

class MyClass {
  var x: int = 21;
}

proc main() {

  var myOwned = new owned MyClass();
  var myBorrow = myOwned.borrow();
  var myUnmanaged = new unmanaged MyClass();
  defer { delete myUnmanaged; }

  var addr1 = c_addrOf(myOwned);
  var addr2 = c_addrOf(myBorrow);
  var addr3 = c_addrOf(myUnmanaged);

  writeln(addr1.deref(), " | ", addr2.deref(), " | ", addr3.deref());
  
  // Verify that c_addrOf on unmanaged class returns the same as c_ptrTo
  writeln(c_addrOf(myUnmanaged):string == c_ptrTo(myUnmanaged):string);
}
