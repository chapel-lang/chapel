use ThrowError;

writeln("should not continue");

try! throwAnError();

writeln("fail: continued");
