//
// Test case of a variety of possible scenarios where an exception could be thrown
// This test makes sure the exception is caught and the program does not hang
// test/library/packages/Python/correctness/unsupportedTypes.chpl tests something similar
//


use Python;
var i = new Interpreter();

try {
  var f = new Function(i, "lambda x,:");
} catch e {
  writeln("Caught exception: ", e);
}

try {
  var fakePklData: bytes = b"fakePklData";
  var v = new Value(i, fakePklData);
} catch e {
  writeln("Caught exception: ", e);
}

try {
  var fakeBytecode: bytes = b"fakeBytecode";
  var m = new Module(i, "foo", fakeBytecode);
} catch e {
  writeln("Caught exception: ", e);
}

try {
  var m = new Module(i, "foo", "I am a syntax error!");
} catch e {
  writeln("Caught exception: ", e);
}
