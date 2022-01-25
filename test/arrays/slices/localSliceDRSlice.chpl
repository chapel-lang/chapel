module Spam {
    use BlockDist;
    use IO;
    use IO.FormattedIO;
    use Reflection;

    proc sliceAndDice(fn: string, line: int, ref arr) throws {
        for l in arr.targetLocales do on l {
          for d in arr.localSubdomains() {
//            if !domainDistIsLayout(arr.domain) {
              ref arrSlice = arr.localSlice((...d.dims()));
              writeln("%s,%i:%t".format(fn, line, arrSlice));
//            }
            }
        }
    }

    proc spamInit() throws {
        const Space = {0..#16};
        var A: [Space] int = Space;
        var distA = newBlockArr(Space, int);
        distA = A;

        sliceAndDice(getRoutineName(), getLineNumber(), A);
        sliceAndDice(getRoutineName(), getLineNumber(), distA);
        sliceAndDice(getRoutineName(), getLineNumber(), distA[0..#8]);
        sliceAndDice(getRoutineName(), getLineNumber(), A[0..#8]);
    }


    proc main() : int {
        try! {
            spamInit();
        }

        return 0;
    }
}

