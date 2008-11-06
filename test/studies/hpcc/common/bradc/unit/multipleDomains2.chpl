use BlockDist;

var myLocalesDom = [0..2];
var myLocales: [i in myLocalesDom] locale = Locales(i%numLocales);

const ProblemDist = new Block1D(idxType=int, bbox=[1..10], targetLocales=myLocales);

const Dom1: domain(1) distributed(ProblemDist) = [0..11],
      Dom2: domain(1) distributed(ProblemDist) = [1..12],
      Dom3: domain(1) distributed(ProblemDist) = [-1..10];

writeln("ProblemDist =\n", ProblemDist);
writeln();
