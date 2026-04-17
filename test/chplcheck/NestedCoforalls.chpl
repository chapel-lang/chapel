module Nested {
  proc myProc() {
    coforall tid in 1..8 {
      coforall tid2 in 1..8 {
        writeln(tid + tid2);
      }
    }

    for i in 1..10 {
      coforall tid in 1..8 {
        writeln(i + tid);
      }
    }

    coforall outer in 1..8 {
      for i in 1..10 {
        coforall tid in 1..8 {
          writeln(outer + i + tid);
        }
      }
    }

    coforall loc in Locales do on loc {
      coforall tid in 0..#loc.maxTaskPar {
        writeln(loc, " ", tid);
      }
    }
  }
}
