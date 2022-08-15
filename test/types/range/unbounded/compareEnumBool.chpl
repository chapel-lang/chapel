enum color {red, green, blue};
use color;

var rgb = red..blue;
var rdd = red..;
var ddb = ..blue;
var empty: range(color, BoundedRangeType.boundedNone) = ..;

writeln(rgb == rdd);
writeln(rgb == ddb);
writeln(rgb == empty);

writeln(rdd == ddb);
writeln(rdd == empty);

writeln(ddb == empty);


{
var ft = false..true;
var fdd = false..;
var ddt = ..true;
var empty: range(bool, BoundedRangeType.boundedNone);

writeln(ft == fdd);
writeln(ft == ddt);
writeln(ft == empty);

writeln(fdd == ddt);
writeln(fdd == empty);

writeln(ddt == empty);
}
