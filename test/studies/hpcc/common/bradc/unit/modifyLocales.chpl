use BlockDist;

var myLocalesDom = [0..3];
var myLocales: [i in myLocalesDom] locale = Locales(i%numLocales);

writeln("myLocales = ", myLocales);

//
// TODO: That int(64) is really unfortunate.  We really need param
// domains and ranges and the obvious conversions between them.
// 
const ProblemDist = new Block1D(bbox=[1..10:int(64)], targetLocales=myLocales);

writeln("ProblemDist =\n", ProblemDist);
writeln();

myLocalesDom = [0..2];

writeln("myLocales = ", myLocales);

writeln("ProblemDist =\n", ProblemDist);
