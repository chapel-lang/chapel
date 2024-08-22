
var D = {1..5};

var SD: sparse subdomain( D );                    // warning: 'sparse' is unstable
writeln(SD);

var DD = D dmapped new dmap(new DefaultDist());   // warning: 'dmapped' is unstable
writeln(DD);

var dm = new dmap(new DefaultDist());
testit(dm);

proc testit(arg: _distribution(?)) {
  compilerError("done");
}

// NB this overload does not match the above call testit(dm)
// because by resolution time dm's dmap has been converted to '_distribution'
// while this overload's dmap is still 'dmap'.
proc testit(arg: dmap) {
  compilerError("arg: dmap");
}
