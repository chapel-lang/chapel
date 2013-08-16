use BlockDist;

config const n = 10;

config const epsilon = 0.01;

config const printArrays = false;

const LocDom = {1..n  , 1..n  },
         Dom = LocDom dmapped Block(LocDom),
      BigDom = {0..n+1, 0..n+1} dmapped Block(LocDom);

const LocaleGridDom = Dom._value.dist.targetLocDom,
      LocaleGrid = Dom._value.dist.targetLocales;

writeln("Our locale grid is as follows:\n", LocaleGrid, "\n");

coforall (lr,lc) in LocaleGridDom {
  on LocaleGrid[lr,lc] {
    writeln("Hello from locale #", here.id, " at ", (lr,lc));
  }
}

for (lr,lc) in LocaleGridDom {
  on LocaleGrid[lr,lc] {
    writeln("locale #", here.id, " owns ", Dom._value.locDoms[lr,lc].myBlock);
  }
}

var takeTurns$: sync int = 0;
var delta$: sync real = 0;

class DomArr {
  var Dom: domain(2);
  var Arr: [Dom] real;
}

var LocalDomArrs: [LocaleGridDom] DomArr;

var numIters = 0;

coforall (lr,lc) in LocaleGridDom {
  on LocaleGrid[lr,lc] {
    const MyLocDom = Dom._value.locDoms[lr,lc].myBlock;
    const WithFluff = MyLocDom.expand(1);

    var MyDomArr = new DomArr(Dom=WithFluff);
    LocalDomArrs[lr,lc] = MyDomArr;

    const PanelDom = {-1..1, -1..1};
    const Panels: [PanelDom] domain(2);

    for ij in PanelDom {
      Panels[ij] = MyLocDom.exterior(ij);
    }

    while (takeTurns$.readXX() != here.id) { }
    writeln("locale #", here.id, " panels:\n");
    writeln(Panels);
    takeTurns$.writeXF((here.id + 1)%numLocales);

    writeln("locale #", here.id, " with fluff owns: ", WithFluff);

    /*
        A[  n/4+1,   n/4+1] =  1.0;
        A[3*n/4+1, 3*n/4+1] =  1.0;
        A[  n/4+1, 3*n/4+1] = -1.0;
        A[3*n/4+1,   n/4+1] = -1.0;
    */

    const p1 = (  n/4+1,   n/4+1);
    const p2 = (3*n/4+1, 3*n/4+1);
    const p3 = (  n/4+1, 3*n/4+1);
    const p4 = (3*n/4+1,   n/4+1);

    var A => MyDomArr.Arr;
    var B: [WithFluff] real;

    if WithFluff.member(p1) then
      A[p1] =  1.0;

    if WithFluff.member(p2) then
      A[p2] =  1.0;

    if WithFluff.member(p3) then
      A[p3] =  -1.0;

    if WithFluff.member(p4) then
      A[p4] =  -1.0;


    while (takeTurns$.readXX() != here.id) { }
    writeln("locale #", here.id, "'s slab:\n", A[MyLocDom]);
    takeTurns$.writeXF((here.id + 1)%numLocales);

    do {
      for ij in PanelDom {
        if (ij != (0,0)) {
          var neighbor = (lr,lc)+ij;

          if (neighbor(1) < 0 || neighbor(2) < 0 ||
              neighbor(1) > LocaleGridDom.high(1) ||
              neighbor(2) > LocaleGridDom.high(2)) then
            // out of bounds
            continue;

          if (Dom[Panels[ij]].numIndices > 0) {
            // Update fluff
            A[Panels[ij]] = LocalDomArrs[neighbor].Arr[Panels[ij]];
          }
        }
      }

      /*
      while (takeTurns$.readXX() != here.id) { }
      writeln("locale #", here.id, "'s slab:\n", A[MyLocDom]);
      takeTurns$.writeXF((here.id + 1)%numLocales);
      */

      forall (i,j) in MyLocDom do
        B[i,j] = 0.25   * A[i,j]
               + 0.125  * (A[i+1,j  ] + A[i-1,j  ] + A[i  ,j-1] + A[i  ,j+1])
               + 0.0625 * (A[i-1,j-1] + A[i-1,j+1] + A[i+1,j-1] + A[i+1,j+1]);

      /*
      while (takeTurns$.readXX() != here.id) { }
      writeln("locale #", here.id, "'s slab:\n", A[MyLocDom]);
      takeTurns$.writeXF((here.id + 1)%numLocales);
      */

      // Need to wait until everyone has read each others' A's until we
      // can swap in the following loop.

      //
      // Using this as sort of a poor man's barrier/fence
      //
      while (takeTurns$.readXX() != here.id) { }
      takeTurns$.writeXF((here.id + 1)%numLocales);
      while (takeTurns$.readXX() != here.id) { }
      takeTurns$.writeXF((here.id + 1)%numLocales);

      // could parallelize if we made the reduction safe
      var locDelta = 0.0;
      for ij in MyLocDom {
        const diff = fabs(B[ij] - A[ij]);

        if (diff > locDelta) then
          locDelta = diff;

        B[ij] <=> A[ij];
      }

      while (takeTurns$.readXX() != here.id) { }
      const prevDelta = delta$;
      if (locDelta > prevDelta) then
        delta$ = locDelta;
      else
        delta$ = prevDelta;
      takeTurns$.writeXF((here.id + 1)%numLocales);

      while (takeTurns$.readXX() != here.id) { }
      const done = (delta$.readXX() <= epsilon);
      takeTurns$.writeXF((here.id + 1)%numLocales);

      if here.id == numLocales-1 {
        delta$.writeXF(0);
        numIters += 1;
      }

    } while (!done);
  }
}

if printArrays {
  var A: [LocDom] real;
  for lda in LocalDomArrs {
    var Interior = lda.Dom.expand(-1);
    A[Interior] = lda.Arr[Interior];
  }
  writeln("A is: ", A);
}

writeln("# iterations: ", numIters);
