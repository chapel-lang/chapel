// Similar to indef4.chpl, but used to test garbage collection.
// Test growing and shrinking an indefinite domain.

use Memory;

def jam() {
  var ITERATIONS = 5;
  var SIZE = 100000;

  var idomain: domain(uint);

  def icheck( i) {
    writeln( "member(", i, ") = ", idomain.member( i));
  }

  for i in 1..ITERATIONS {
    for d in 1..2*SIZE {       // empty domain
      idomain.remove(d:uint);
    }

    for d in 1..SIZE {         // add everybody
      idomain.add(d:uint);
    }

    for d in 1..SIZE/2 {       // delete half
      idomain.remove(d:uint);
    }

    for d in SIZE+1..2*SIZE {  // add some
      idomain.remove(d:uint);
    }

    for d in 1..SIZE {         // try to add some existing
      if (d%2 == 0) then
        idomain.add(d:uint);
    }

    for d in SIZE/2..SIZE {    // delete some
      idomain.remove(d:uint);
    }
  }
}


var before = memoryUsed();
jam();
var after = memoryUsed();
writeln( "leaked ", after-before, " bytes");
