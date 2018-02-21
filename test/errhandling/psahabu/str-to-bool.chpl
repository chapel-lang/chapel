var x = "true";
var s = "brad";

var x_b = x: bool;
var tmp_b: bool;

try! {
  var s_b = s: bool;
  tmp_b = x_b && s_b;
} catch e: IllegalArgumentError {
  writeln("caught cast error");
  writeln(e.message());
}
