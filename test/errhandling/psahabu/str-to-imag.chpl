var x: string = "9i";
var s: string = "brad";

var x_im = x: imag;
writeln("successful cast");

try! {
  var s_im = s: imag;
  x_im += s_im;
} catch e: IllegalArgumentError {
  writeln("caught cast error");
  writeln(e.message());
}
