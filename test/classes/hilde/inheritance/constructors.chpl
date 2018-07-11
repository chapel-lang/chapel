// constructors.chpl
//
// Test calling the constructor in a derived class.
//

class base {
  var _i:int;
  // If you comment out this constructor, the test will compile and run.
  proc base(i = -1) {
    _i = i;
  }
}

class sub : base {
  var _j:int;
  proc sub(i:int, j = -2) {
    // How do we call the base class constructor?
    _i = i;
    _j = j;
  }
}

proc main() {
  var p = new unmanaged sub(3,4);
  var q = new unmanaged base(2);

  writeln(p._i, p._j);
  writeln(q._i);

  delete q;
  delete p;
}
