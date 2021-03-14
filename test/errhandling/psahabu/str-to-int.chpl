var x: string = "5";
var s: string = "brad";

var x_i = x: int;
writeln("successful cast");

try! {
  var s_i = s: int;
  x_i += s_i;
} catch e: IllegalArgumentError {
  writeln("caught cast error");
  writeln(e.message());
}
