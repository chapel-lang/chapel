use Debugger;
use CTypes;

proc main() {

  // DWARFDUMP: myStarTupInt
  // DWARFDUMP: myStarTupBool
  // DWARFDUMP: myStarTupReal
  var myStarTupInt: 3*int(32) = (1:int(32), 2:int(32), 3:int(32));
  var myStarTupBool: 2*bool = (true, false);
  var myStarTupReal: 1*real;
  writeln(myStarTupInt, myStarTupBool, myStarTupReal, sep =" | ");

  // DWARFDUMP: myHeteroTup
  // DWARFDUMP: (int(64),real(64),string)
  // DWARFDUMP: myHeteroTup2
  // DWARFDUMP: (int(64),real(64),string,bool,(int(64),real(64),string),3*int(32))
  var myHeteroTup: (int, real, string) = (42, 3.14, "hello");
  var myHeteroTup2 = (1, 2.0, "three", false, myHeteroTup, myStarTupInt);
  writeln(myHeteroTup, myHeteroTup2, sep =" | ");

  // TODO: add classes in tuples

  breakpoint;
}
