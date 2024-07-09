use BlockDist;

const Dom = {1..10};
const Space = Dom dmapped new blockDist(Dom);
var A : [Space] int;

ref B = A;

proc main() {

  on Locales[numLocales-1] {
    for i in B.localSubdomain() do local {
      B[i] += 1;
    }
  }

  writeln(B);
}
