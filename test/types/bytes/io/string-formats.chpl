use IO;

var b = "bytes":bytes;

try! {
  stdout.writef("%s\n", b);
}
catch e: SystemError {
  writeln("Caught the correct error");
}

try! {
  stdout.writef("%S\n", b);
}
catch e: SystemError {
  writeln("Caught the correct error");
}
