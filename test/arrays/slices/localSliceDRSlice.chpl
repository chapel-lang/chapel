module Spam {
    use BlockDist;
    use IO;
    use IO.FormattedIO;
    use Reflection;

    proc sliceAndDice(fn: string, line: int, ref arr) throws {
        for l in arr.targetLocales do on l {
          for d in arr.localSubdomains() {
            {
              // test domain-based slicing
              ref arrSlice = arr.localSlice(d);
              writeln("%s,%i:%t".format(fn, line, arrSlice));
            }
            {
              // test range-based slicing
              ref arrSlice = arr.localSlice((...d.dims()));
              writeln("%s,%i:%t".format(fn, line, arrSlice));
            }
          }
        }
    }

    proc spamInit() throws {
      {
        const Space = {0..#16};
        var A: [Space] int = Space;
        var distA = newBlockArr(Space, int);
        distA = A;

        sliceAndDice(getRoutineName(), getLineNumber(), A);
        sliceAndDice(getRoutineName(), getLineNumber(), distA);
        sliceAndDice(getRoutineName(), getLineNumber(), distA[0..#8]);
        sliceAndDice(getRoutineName(), getLineNumber(), A[0..#8]);
      }
      {
        const Space = {1..4, 1..4};
        var A: [Space] int = [(i,j) in Space] (i-1)*4 + j;
        var distA = newBlockArr(Space, int);
        distA = A;

        sliceAndDice(getRoutineName(), getLineNumber(), A);
        sliceAndDice(getRoutineName(), getLineNumber(), distA);
        sliceAndDice(getRoutineName(), getLineNumber(), distA[1..2, 1..2]);
        sliceAndDice(getRoutineName(), getLineNumber(), A[1..2, 1..2]);
      }
    }


    proc main() : int {
        try! {
            spamInit();
        }

        return 0;
    }
}

