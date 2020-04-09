use BradsBlock1DPar;

var myLocalesDom = {0..2};
var myLocales: [myLocalesDom] locale = [i in myLocalesDom] Locales(i%numLocales);

//
// The following is the manual rewrite of something like:
//
//   const ProblemDist = new Block1DDist(bbox={1..10}, targetLocales=myLocales);
//
//   const Dom1: domain(1) dmapped(ProblemDist) = {0..11},
//         Dom2: domain(1) dmapped(ProblemDist) = {1..12},
//         Dom3: domain(1) dmapped(ProblemDist) = {-1..10};
//
// TODO: The following cast to int(64) is really unfortunate.  We
// really need param domains and ranges and the obvious conversions
// between them.
// 
const ProblemDist = new unmanaged Block1DDist(bbox={1..10:int(64)}, targetLocales=myLocales);

writeln("ProblemDist =\n", ProblemDist);
writeln();

const Dom1 = ProblemDist.newDomain({0..11:int(64)});
const Dom2 = ProblemDist.newDomain({1..12:int(64)});
const Dom3 = ProblemDist.newDomain({-1..10:int(64)});

//
// The following loop is the manual rewrite of:
//   forall ijk in (Dom1, Dom2, Dom3) {
//

// This would be a coforall, but making it a for for debugging purposes:
for block in Dom1.newThese(IteratorType.leader) {
  //
  // TODO: This on clause doesn't do what I intended, probably for
  // good reasons.  Moving the parallelism and on clause into the
  // leader should solve this issue.
  // 
  on block {
    writeln("locale ", here, " is being asked to follow ", block);
    for ijk in zip(Dom1.newThese(IteratorType.follower, block),
                Dom2.newThese(IteratorType.follower, block),
                Dom3.newThese(IteratorType.follower, block)) {
      writeln("ijk = ", ijk);
    }
  }
}
writeln();


//
// The following loops are rewrites that wouldn't happen in practice
// (the followers would change order), but are written to be easy to
// read and ensure that anyone can lead correctly:
//
for block in Dom2.newThese(IteratorType.leader) {
  on block {
    writeln("locale ", here, " is being asked to follow ", block);
    for ijk in zip(Dom1.newThese(IteratorType.follower, block),
                Dom2.newThese(IteratorType.follower, block),
                Dom3.newThese(IteratorType.follower, block)) {
      writeln("ijk = ", ijk);
    }
  }
}
writeln();


for block in Dom3.newThese(IteratorType.leader) {
  on block {
    writeln("locale ", here, " is being asked to follow ", block);
    for ijk in zip(Dom1.newThese(IteratorType.follower, block),
                Dom2.newThese(IteratorType.follower, block),
                Dom3.newThese(IteratorType.follower, block)) {
      writeln("ijk = ", ijk);
    }
  }
}
writeln();

delete Dom3, Dom2, Dom1;
delete ProblemDist;
