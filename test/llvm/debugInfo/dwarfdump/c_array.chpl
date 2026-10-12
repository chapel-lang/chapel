use Debugger;
use CTypes;

// DWARFDUMP: myRec
record myRec {
  var x: int;
}

proc main() {

  // DWARFDUMP: myArrayInt
  // DWARFDUMP: c_array(int(32),10)
  // DWARFDUMP: myArrayBool
  // DWARFDUMP: c_array(bool,5)
  // DWARFDUMP: myArrayReal
  // DWARFDUMP: c_array(real(64),3)
  // DWARFDUMP: myArrayRec
  // DWARFDUMP: c_array(myRec,16)
  var myArrayInt: c_array(int(32), 10);
  var myArrayBool: c_array(bool, 5);
  var myArrayReal: c_array(real, 3);
  var myArrayRec: c_array(myRec, 16);

  // DWARFDUMP: myNestedArray
  // DWARFDUMP: c_array(c_array(int(32),4),2)
  // DWARFDUMP: myPtrArray
  // DWARFDUMP: c_array(c_ptr(int(32)),8)
  var myNestedArray: c_array(c_array(int(32), 4), 2);
  var myPtrArray: c_array(c_ptr(int(32)), 8);

  writeln(myArrayInt, myArrayBool, myArrayReal, myArrayRec, sep =" | ");
  writeln(myNestedArray, myPtrArray, sep =" | ");
  breakpoint;

}
