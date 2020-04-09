//Helper functions for stencil.chpl

public use stencil;
private use IO;

// verboseness
config const v1 = true, v2 = true;
proc msg1(args...) { if v1 then writeln((...args)); }
proc msg2(args...) { if v2 then writeln((...args)); }

// gridLocales setup
var manylocs: bool;
proc setupGridLocales(ensureManyLocs = false) {
  var gridLocales: [gridDom] locale;
  manylocs = (numLocales >= gridLocales.size);

  if manylocs {
    var i = 0;
    for ml in gridLocales { ml = Locales(i); i += 1; }
    //write("set up with ", i, " distinct locales");
    if i < numLocales then write(" out of ", numLocales);
    //writeln();
  } else {
    gridLocales = Locales(0);
    //writeln("oversubscribed Locales(0) over ", gridLocales.size, " locales");
  }

  if !manylocs && ensureManyLocs then halt("not enough locales: wanted ",
    gridLocales.size, ", got ", numLocales);
  writeln();

  return gridLocales;
}

// show what we have
config const vcalc = false, vsend = false;
const fpstyle = new iostyle(realfmt = 1, precision = 2, min_width_columns = 6);

proc showme(oddphase: bool, delta: elType, msg = "") {
  if !vcalc then return;
  writeln(msg, "  delta ", delta, if oddphase then "  (B->A)" else "  (A->B)");
  writeln();

  for (gix, dat) in zip(gridDom, Data) {
    writeln("Grid point ", gix, " on locale ", dat.locale.id);
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


proc showref(oddphase: bool, msg = "") {
  if !vcalc then return;
  writeln(msg);
  writeln();

  if oddphase {
    writeln("refdataA =");
    writeln(refdataA, fpstyle);
  } else {
    writeln("refdataB =");
    writeln(refdataB, fpstyle);
  }
  writeln();
}


//const showdummy: [0..n+1, 0..m+1] eltype;
const colsep = "  ";
proc showdummyrow() {
  const width = (m+2) * fpstyle.min_width_columns;
  for 1..width do write(" ");
}
proc showrealrow(oddphase: bool, gi:int, gj:int, i:int) {
  if oddphase then
    showrealrow(Data[gi,gj].B, i);
  else
    showrealrow(Data[gi,gj].A, i);
}
proc showrealrow(ARR, i:int) {
  for j in 0..m+1 do write(ARR[i,j], fpstyle);
}

proc showfetch(oddphase: bool, msg = "") {
  if !vsend then return;
  writeln(msg, if oddphase then " (fetch B->B)" else " (fetch A->A)");
  writeln();

  const innerGrid = { if g <= 2 then 1..1 else 2..g-1,
                      if h <= 2 then 1..1 else 2..h-1 };
  for ((gi,gj), dat) in zip(innerGrid, Data[innerGrid]) {
    writeln("Grid point ", (gi, gj), " on locale ", dat.locale.id);
    // first row
    if gi > 1 {
      for i in 0..n+1 {
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
      for i in 0..n+1 {
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
      for i in 0..n+1 {
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

      forall ((i,j), a, b1, b2, b3, b4) in zip(dat.domCompute, dat.Acompute,
        dat.B[dat.domCompute.translate(adjcoords(1))],
        dat.B[dat.domCompute.translate(adjcoords(2))],
        dat.B[dat.domCompute.translate(adjcoords(3))],
        dat.B[dat.domCompute.translate(adjcoords(4))])
      {
        a = (b1 + b2 + b3 + b4) / 4;
      }

    } else { //  !oddphase

      forall ((i,j), a, b1, b2, b3, b4) in zip(dat.domCompute, dat.Bcompute,
        dat.A[dat.domCompute.translate(adjcoords(1))],
        dat.A[dat.domCompute.translate(adjcoords(2))],
        dat.A[dat.domCompute.translate(adjcoords(3))],
        dat.A[dat.domCompute.translate(adjcoords(4))])
      {
        a = (b1 + b2 + b3 + b4) / 4;
      }

    } // if oddphase

    dat.localDelta = max reduce [(a,b) in zip(dat.Acompute, dat.Bcompute)] abs(a-b);
  } // forall dat

  delta =  max reduce [dat in Data] dat.localDelta;
}
