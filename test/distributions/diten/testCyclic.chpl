use Cyclic;

var myLocalesDom = {1..3};
var myLocales: [myLocalesDom] locale = [i in myLocalesDom] Locales(i%numLocales);

writeln("myLocales = ", myLocales);

//
// TODO: That int(64) is really unfortunate.  We really need param
// domains and ranges and the obvious conversions between them.
// 
const ProblemDist = new unmanaged Cyclic1DDist(targetLocales=myLocales);
const ProblemDom = ProblemDist.newDomain({-5..5:int(64)});

var A = ProblemDom.newArray(real);
var B = ProblemDom.newArray(real);
var C = ProblemDom.newArray(real);

var i = 0;

for (b, c) in zip(B, C) {
  b = i;
  c = i+1;
  i += 1;
}

writeln(A);
writeln(B);
writeln(C);

var alpha = 1.0;

for block in A.newThese(IteratorType.leader) {
  writeln("block is: ", block);
  on block {
    var myalpha = alpha;
    for (a,b,c) in zip(A.newThese(IteratorType.follower, block),
                       B.newThese(IteratorType.follower, block),
                       C.newThese(IteratorType.follower, block)) {
      a = b + myalpha * c;
    }
  }
}

writeln(A);
writeln(B);
writeln(C);

delete C;
delete B;
delete A;
delete ProblemDom;
delete ProblemDist;
