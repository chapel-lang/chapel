
var D = {1..5};

var SD: sparse subdomain( D );      // warning: 'sparse' is unstable
writeln(SD);

var DD = D dmapped DefaultDist();   // warning: 'dmapped' is unstable
writeln(DD);

compilerError("done");
