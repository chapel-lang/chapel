var x: string = "1 + 9i";
var s: string = "brad";

var x_c = x: complex;
writeln("successful cast");

try! {
  var s_c = s: complex;
  x_c += s_c;
} catch e: IllegalArgumentError {
  writeln("caught cast error");
  writeln(e.message());
}
