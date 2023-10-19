enum color {red, green, blue};
use color;

var rgb = red..blue;
var rdd = red..;
var ddb = ..blue;
var negrgb = red..blue by -1;
var negrdd = red.. by -1;
var negddb = ..blue by -1;
var dd: range(color, boundKind.neither);

writeln("equalities");

writeln(rgb == rdd);
writeln(rgb == ddb);
writeln(rgb == dd);

writeln(rdd == ddb);
writeln(rdd == dd);

writeln(ddb == dd);
writeln(negrgb == negrdd);
writeln(negrdd == negddb);
writeln(negrdd == dd);

writeln("inequalities");

writeln(rgb != rdd);
writeln(rgb != ddb);
writeln(rgb != dd);

writeln(rdd != ddb);
writeln(rdd != dd);

writeln(ddb != dd);
writeln(negrgb != negrdd);
writeln(negrdd != negddb);
writeln(negrdd != dd);

{
var ft = false..true;
var fdd = false..;
var ddt = ..true;
var negft = false..true by -1;
var negfdd = false.. by -1;
var negddt = ..true by -1;
var dd: range(bool, boundKind.neither);

writeln("equalities");

writeln(ft == fdd);
writeln(ft == ddt);
writeln(ft == dd);

writeln(fdd == ddt);
writeln(fdd == dd);

writeln(ddt == dd);
writeln(negft == negfdd);
writeln(negfdd == negddt);
writeln(negfdd == dd);

writeln("inequalities");

writeln(ft != fdd);
writeln(ft != ddt);
writeln(ft != dd);

writeln(fdd != ddt);
writeln(fdd != dd);

writeln(ddt != dd);
writeln(negft != negfdd);
writeln(negfdd != negddt);
writeln(negfdd != dd);
}
