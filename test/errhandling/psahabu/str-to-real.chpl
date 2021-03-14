var x: string = "3.1415";
var s: string = "brad";

var x_r = x: real;
writeln("successful cast");

try! {
  var s_r = s: real;
  x_r += s_r;
} catch e: IllegalArgumentError {
  writeln("caught cast error");
  writeln(e.message());
}
