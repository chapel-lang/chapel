// This is analogous to "skyline arrays", where an array's elements
// are domains instead of arrays. Cf.
//   test/arrays/ferguson/from-iterator/skyline.chpl

config const n = 2;
var r = 1..n;

proc makeDomain(i:int) {
  return {1..i};
}

// AA is an array of domains
var AA = for i in r do makeDomain(i);
for subdomainn in AA {
  writeln("subdomain ", subdomainn);
  var B:subdomainn.type;
  writeln("subdomain (B) ", B);
}

writeln("AA domain ", AA.domain);
var element:AA.eltType;
writeln("AA (element) eltType domain ", element);

// the same with an associative domain

var rr = {"hi", "there"};
proc makeDomain(s:string) {
  return {1..s.size};
}
proc makeDomain(lowerBound:int, s:string) {
  return {lowerBound..s.size};
}

// a forall expression
var AAA = [s in rr] makeDomain(s);
writeln("AAA.domain ", AAA.domain.sorted());
writeln("AAA = ", AAA.sorted(new AAcomparator()));

// a promoted expression
var AAB = makeDomain(2, rr);
writeln("AAB.domain ", AAB.domain.sorted());
writeln("AAB = ", AAB.sorted(new AAcomparator()));

record AAcomparator { proc key(a) return a.size; }
