use BlockDist;

var myLocalesDom = {0..2};
var myLocales: [myLocalesDom] locale = [i in myLocalesDom] Locales(i%numLocales);

const ProblemDist = new dmap(new Block(rank=1, idxType=int, boundingBox={1..10}, targetLocales=myLocales, dataParTasksPerLocale=1));

const Dom1: domain(1) dmapped(ProblemDist) = {0..11},
      Dom2: domain(1) dmapped(ProblemDist) = {1..12},
      Dom3: domain(1) dmapped(ProblemDist) = {-1..10};

writeln("ProblemDist =\n", ProblemDist);
writeln();
