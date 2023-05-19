use BlockDist;

var targetLocDom = {0..numLocales-2};
var targetLocales = Locales[targetLocDom];

var n = 10;
var B = new dmap(new Block({1..n}, targetLocales));
var D = {1..n} dmapped B;
var A: [D] real;

targetLocales[numLocales-2] = Locales[numLocales-1];

B = new dmap(new Block({1..2*n}, targetLocales)); // error b/c targets differ

inspectDist(A);

proc inspectDist(X: [] ?t) {
  writeln("-------------");
  for loc in Locales do
    on loc {
      writeln(here.id, ": ", X.localSubdomain(), " from ", X.domain);

      // Let's be quite sure these match...
      if D.localSubdomain() != X.localSubdomain() then
        writeln(here.id, ": ", D.localSubdomain(), " from ", D);
      if X.domain.localSubdomain() != X.localSubdomain() then
        writeln(here.id, ": ", X.domain.localSubdomain(), " from ", X.domain);
    }
  writeln("-------------");
}

