
use BlockDist;

proc main() {
  assert(numLocales >= 2, "test requires at least two locales");
  const alpha = [Locales[0], Locales[1]];
  const beta =  [Locales[1], Locales[0]];
  const D = {1..100};
  const AS = D dmapped Block(D, targetLocales=alpha);
  const BS = D dmapped Block(D, targetLocales=beta);
  var A : [AS] int;
  var B : [BS] int;

  A = 1;
  B = 2;

  forall (a, b) in zip(A[A.localSubdomain()], B[local]) {
    writeln(a + b);
  }
}
