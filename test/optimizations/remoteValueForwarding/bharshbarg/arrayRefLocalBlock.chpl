use BlockDist;

const Dom = {1..10};
const Space = Dom dmapped Block(Dom);
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
