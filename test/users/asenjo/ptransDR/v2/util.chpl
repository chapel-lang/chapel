//Helper functions for stencil.chpl

use ptrans;

var manylocs: bool;
proc setupGridLocales(ensureManyLocs = false) {
  manylocs = (numLocales >= gridLocales.numElements);

  if manylocs {
    var i = 0;
    for ml in gridLocales { ml = Locales(i); i += 1; }
    write("set up with ", i, " distinct locales");
    if i < numLocales then write(" out of ", numLocales);
    writeln();
  } else {
    gridLocales = Locales(0);
    writeln("oversubscribed Locales(0) over ", gridLocales.numElements, " locales");
  }

  if !manylocs && ensureManyLocs then halt("not enough locales: wanted ",
    gridLocales.numElements, ", got ", numLocales);
  writeln();
}

// show what we have
config const vcalc = true, vsend = true;
const fpstyle = new iostyle(realfmt = 1, precision = 2, min_width = 6);

proc showme(oddphase: bool, msg = "") {
  if !vcalc then return;

  for (gix, dat) in (gridDom, Data) {
    writeln("Transposed matrices");
    if oddphase {
      writeln("B =");
      writeln(dat.B, fpstyle);
      writeln("A =");
      writeln(dat.A, fpstyle);
    } else {
      writeln("A =");
      writeln(dat.A, fpstyle);
      writeln("B =");
      writeln(dat.B, fpstyle);
    }
    writeln();
  }
}


//const showdummy: [1..n, 1..m] eltype;
const colsep = "  ";
proc showdummyrow() {
  const width = (m) * fpstyle.min_width;
  for 1..width do write(" ");
}
proc showrealrow(oddphase: bool, gi:int, gj:int, i:int) {
  if oddphase then
    showrealrow(Data[gi,gj].B, i);
  else
    showrealrow(Data[gi,gj].A, i);
}
proc showrealrow(ARR, i:int) {
  for j in 1..m do write(ARR[i,j], fpstyle);
}

proc showfetch(oddphase: bool, msg = "") {
  if !vsend then return;
  writeln(msg, if oddphase then " (fetch B->B)" else " (fetch A->A)");
  writeln();

  const innerGrid = { if g <= 2 then 1..1 else 2..g-1,
                      if h <= 2 then 1..1 else 2..h-1 };
  for ((gi,gj), dat) in (innerGrid, Data[innerGrid]) {
    writeln("TRANSPOSED MATRICES");
    // first row
    if gi > 1 {
      for i in 1..n {
        if gj > 1 then showrealrow(oddphase, gi-1, gj-1, i); else showdummyrow();
        write(colsep);
        showrealrow(oddphase, gi-1, gj, i);
        write(colsep);
        if gj < h then showrealrow(oddphase, gi-1, gj+1, i); else showdummyrow();
        writeln();
      }
      writeln("-----------");
    } else {
      writeln("no first row");
    }
    // second row - always there
    if true {
      for i in 1..n {
        if gj > 1 then showrealrow(oddphase, gi, gj-1, i); else showdummyrow();
        write(colsep);
        showrealrow(oddphase, gi, gj, i);
        write(colsep);
        if gj < h then showrealrow(oddphase, gi, gj+1, i); else showdummyrow();
        writeln();
      }
      writeln("-----------");
    }
    // third row
    if gi < g {
      for i in 1..n {
        if gj > 1 then showrealrow(oddphase, gi+1, gj-1, i); else showdummyrow();
        write(colsep);
        showrealrow(oddphase, gi+1, gj, i);
        write(colsep);
        if gj < h then showrealrow(oddphase, gi+1, gj+1, i); else showdummyrow();
        writeln();
      }
    } else {
      writeln("no third row");
    }
    writeln();
  } // for gridDom

  writeln("done ", msg);
  writeln();
}

proc computeOld(oddphase: bool, out delta: elType) {
  forall dat in Data {

    if oddphase {

      forall ((i,j), a, b1, b2, b3, b4) in (dat.domCompute, dat.Acompute,
        dat.B[dat.domCompute.translate(adjcoords(1))],
        dat.B[dat.domCompute.translate(adjcoords(2))],
        dat.B[dat.domCompute.translate(adjcoords(3))],
        dat.B[dat.domCompute.translate(adjcoords(4))])
      {
        a = (b1 + b2 + b3 + b4) / 4;
      }

    } else { //  !oddphase

      forall ((i,j), a, b1, b2, b3, b4) in (dat.domCompute, dat.Bcompute,
        dat.A[dat.domCompute.translate(adjcoords(1))],
        dat.A[dat.domCompute.translate(adjcoords(2))],
        dat.A[dat.domCompute.translate(adjcoords(3))],
        dat.A[dat.domCompute.translate(adjcoords(4))])
      {
        a = (b1 + b2 + b3 + b4) / 4;
      }

    } // if oddphase

    dat.localDelta = max reduce [(a,b) in (dat.Acompute, dat.Bcompute)] abs(a-b);
  } // forall dat

  delta =  max reduce [dat in Data] dat.localDelta;
}
