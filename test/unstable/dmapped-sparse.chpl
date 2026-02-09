
var D = {1..5};

var SD: sparse subdomain( D );                    // warning: 'sparse' is unstable
writeln(SD);

var DD = D dmapped defaultDist;   // warning: 'dmapped' is unstable
writeln(DD);

var dm = defaultDist;
testit(dm);

proc testit(arg: _distribution(?)) {
  compilerError("done");
}
