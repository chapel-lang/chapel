//Helper functions for stencil.chpl

use stencil;
private use IO;

// verboseness
config const v1 = false, v2 = false;
proc msg1(args...) { if v1 then writeln((...args)); }
proc msg2(args...) { if v2 then writeln((...args)); }

// gridLocales setup
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
config const vcalc = true, vsend = true;
const fpstyle = new iostyle(realfmt = 1, precision = 2, min_width_columns = 6);
