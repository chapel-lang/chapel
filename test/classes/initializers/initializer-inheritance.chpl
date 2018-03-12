// modified from test/classes/hilde/inheritance/constructors.chpl
//
// Test calling the initializer in a derived class.
//

class base {
  var _i:int;

  proc init(i = -1) {
    _i = i;
  }
}

class sub : base {
  var _j:int;
  proc init(i:int, j = -2) {
    super.init(i);
    _j = j;
  }
}

proc main() {
  var p = new sub(3,4);
  var q = new base(2);

  writeln(p._i, p._j);
  writeln(q._i);

  delete q;
  delete p;
}
