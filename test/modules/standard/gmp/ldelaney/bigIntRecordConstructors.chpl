use GMP;

/*
  Things to test: 
  Constructors:
    no arg
    bool/int (init2/nbits)
    int
    string/base
    string/base/error
    BigIntRecord
  Destructor:
    called automatically at end of scope
      to test may need to check for memory leaks?

*/


proc main(){
  var x1 = new BigInt();
  var x2 = new BigInt(true, 8); // TODO: How to test this?
  var x3 = new BigInt(100);
  var x4 = new BigInt("1100101", 2); 
  // TODO: add the string / base / error constructor
  var x6 = new BigInt(x4);
  var x7 : BigInt = x6;
  var x8 = x3; //This one doesn't make a deep copy, but thinks it does
  x1.writeThis(stdout);
  writeln();
  x2.writeThis(stdout);
  writeln();
  x3.writeThis(stdout);
  writeln();
  x4.writeThis(stdout);
  writeln();
  x6.writeThis(stdout);
  writeln();
  x7.writeThis(stdout);
  writeln();
  x8.writeThis(stdout);
  writeln();

}