use BlockDist;

var myLocalesDom = {0..2};
var myLocales: [myLocalesDom] locale = [i in myLocalesDom] Locales(i%numLocales);

writeln("myLocales = ", myLocales);

//
// TODO: That int(64) is really unfortunate.  We really need param
// domains and ranges and the obvious conversions between them.
// 
const ProblemDist = new blockDist(rank=1, idxType=int(64), boundingBox={1..10:int(64)}, targetLocales=myLocales);

writeln("ProblemDist =\n", ProblemDist);
writeln();

myLocalesDom = {0..1};

writeln("myLocales = ", myLocales);

writeln("ProblemDist =\n", ProblemDist);
