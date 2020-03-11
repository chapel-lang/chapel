//Helper functions for stencil.chpl

public use ptrans;
private use IO;

var manylocs: bool;
proc setupGridLocales(ensureManyLocs = false) {
  var gridLocales: [gridDom] locale?;
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

  return gridLocales!;
}

// show what we have
config const vsend = true;
const fpstyle = new iostyle(realfmt = 1, precision = 2, min_width_columns = 6);

const colsep = "  ";
proc showdummyrow() {
  const width = (m) * fpstyle.min_width_columns;
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
  writeln(msg, if oddphase then " (transpose A->B)" else " (transpose B->A)");
  writeln();

  const innerGrid = { if g <= 2 then 1..1 else 2..g-1,
                      if h <= 2 then 1..1 else 2..h-1 };
  for ((gi,gj), dat) in zip(innerGrid, Data[innerGrid]) {
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

  writeln("Done ", msg);
  writeln();
}

