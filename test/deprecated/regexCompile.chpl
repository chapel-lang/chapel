use Regexp;

var r = compile(".*oob.*", utf8=true);  // compiler warning
writeln("foobar".match(r));

try! {
  var r = compile(".*oob.*", utf8=false);  // Illegal argument error
  writeln("foobar".match(r));
}
catch e: IllegalArgumentError {
  writeln("Caught expected error");
}
