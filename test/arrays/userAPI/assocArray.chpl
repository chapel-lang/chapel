use assocArrayAPItest;
use IO;
use ChplFormat;

config const testSlice = false;

proc main() {

  // Ask that arrays are output with [1,2] style
  var output = stdout.withSerializer(new chplSerializer());

  output.writeln([1,2,3]);

  var D:domain(string);
  var A:[D] real;

  D += "zero";
  D += "pointone";
  D += "half";
  D += "one";
  D += "two";
  D += "three";
  D += "four";
  D += "five";
  D += "six";
  D += "seven";
  D += "eight";
  D += "nine";
  D += "ten";

  if !testSlice {
    testAssocArrayAPI(A, output);
  } else {
    const DGood = D;
    D += "eleven";
    D += "twelve";
    D += "thirteen";
    D += "gogol";
    D += "infinity";

    testAssocArrayAPI(A[DGood], output);
  }
}
