use CPtr;
// Tests using it on a generic record
// Note: c_offsetof is intended for use in C interoperability only.
record Test {
  var x;
  var y : int;

}

record Other {
  var a = false;
  var b: uint;
}

var withReal = new Test(1.32, 2);
var withInt = new Test(5, 6);
var withRecord = new Test(new Other(true, 3), 9);

writeln(c_offsetof(withReal.type,"x"));
writeln(c_offsetof(withReal.type,"y"));
writeln(c_offsetof(withInt.type,"x"));
writeln(c_offsetof(withInt.type,"y"));
writeln(c_offsetof(withRecord.type,"x"));
writeln(c_offsetof(withRecord.type,"y"));
writeln(c_offsetof(Test(int),"x"));
writeln(c_offsetof(Test(int),"y"));
