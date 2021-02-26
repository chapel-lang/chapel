proc test(d) {
  var arr: [d] int;

  writeln("Using user-variable: ", d.definedConst);
  writeln("Using instance: ", d._value.definedConst);

  writeln("Array on domain: ", arr);
}

writeln("var domain inited with literal (low level)");
var customVarDom = chpl__buildDomainExpr(1..10, definedConst=false);
test(customVarDom);
writeln();

writeln("const domain inited with literal (low level)");
const customConstDom = chpl__buildDomainExpr(1..10, definedConst=true);
test(customConstDom);
writeln();

writeln("var domain with record initializer");
var varRecInit = new _domain(defaultDist, 1, int, false, false);
test(varRecInit);
writeln();

writeln("const domain with record initializer");
const constRecInit = new _domain(defaultDist, 1, int, false, true);
test(constRecInit);
writeln();
