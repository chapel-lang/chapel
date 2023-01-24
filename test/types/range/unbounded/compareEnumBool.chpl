enum color {red, green, blue};
use color;

var rgb = red..blue;
var rdd = red..;
var ddb = ..blue;
var dd: range(color, BoundedRangeType.boundedNone) = ..;

writeln(rgb == rdd);
writeln(rgb == ddb);
writeln(rgb == dd);

writeln(rdd == ddb);
writeln(rdd == dd);

writeln(ddb == dd);


{
var ft = false..true;
var fdd = false..;
var ddt = ..true;
var dd: range(bool, BoundedRangeType.boundedNone);

writeln(ft == fdd);
writeln(ft == ddt);
writeln(ft == dd);

writeln(fdd == ddt);
writeln(fdd == dd);

writeln(ddt == dd);
}
