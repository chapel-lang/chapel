proc testType(type t) {
  writeln("Testing ", t:string);

  var upper = max(t);
  var lower = min(t);

  var upperAsString = upper:string;
  var lowerAsString = lower:string;

  writeln("Upper limit: ", upperAsString, " (must be ", upper, ")");
  writeln("Lower limit: ", lowerAsString, " (must be ", lower, ")");


  writeln("Can cast upper limit back?: ", upperAsString:t==upper);
  writeln("Can cast lower limit back?: ", lowerAsString:t==lower);

  writeln();
}


testType(int(8));
testType(int(16));
testType(int(32));
testType(int(64));

testType(uint(8));
testType(uint(16));
testType(uint(32));
testType(uint(64));
