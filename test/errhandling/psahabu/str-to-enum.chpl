enum Goal { peace, land, bread }
var x = "peace";
var s = "brad";

var x_g = x: Goal;
writeln("successful cast");

var tmp_g: Goal;

try {
  var s_g = s: Goal;
  tmp_g = s_g;
} catch e {
  writeln("caught cast error");
  writeln(e.message());
}

