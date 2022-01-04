/*
As of this writing (37d7919), formalRequiresTemp() in the compiler
returns different result for the default intent vs. the equivalent
'in' or 'const in' intent. As a result, a formal temp is not inserted
for default-intent formals of primitive types for functions marked "inline".
The generated code is correct nonetheless, although for unrelated reasons,
ex. coerce temps for the case of procs and iterator records for iterators.

This test ensures that this correctness continues.
*/

var AAA: int = 1;
var SSS1,SSS2: sync int;

inline iter myIter(vass1, const in vass2) {
  yield vass1+vass2;
  yield vass1+vass2;
}

inline proc asdfasdf(vass1, const in vass2) {
  writeln("starting asdfasdf: ", (vass1,vass2));
  SSS1.writeEF(1);
  SSS2.readFE();
  writeln("finishing asdfasdf: ", (vass1,vass2));
}

proc fdsa(ref BBB) {
  asdfasdf(BBB, BBB);
}

proc main {

  // testing the case of concurrent modification
  cobegin with (ref AAA) {
    {
      fdsa(AAA);
    }
    {
      SSS1.readFE();
      AAA = 888;
      writeln("set AAA to ", AAA);
      SSS2.writeEF(1);
    }
  }

  // testing the case of an iterator
  for kkk in myIter(AAA, AAA) {
    writeln((kkk,AAA));
    AAA *= 100;
  }
  writeln(AAA);

}
