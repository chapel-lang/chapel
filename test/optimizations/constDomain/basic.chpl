use BlockDist;

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

writeln("var domain inited with literal");
var varDom = {1..10};
test(varDom);
writeln();

writeln("const domain inited with literal");
const constDom = {1..10};
test(constDom);
writeln();

writeln("var domain no init expr minimal type");
var varDomNoInit1: domain(1);
test(varDomNoInit1);
writeln();

writeln("const domain no init expr minimal type");
const constDomNoInit: domain(1);
test(constDomNoInit);
writeln();

writeln("var domain no init expr 2 fields");
var varDomNoInit2: domain(1, int);
test(varDomNoInit2);
writeln();

writeln("const domain no init expr 2 fields");
const constDomNoInit2: domain(1, int);
test(constDomNoInit2);
writeln();

writeln("var domain no init expr");
var varDomNoInit3: domain(1, int, false);
test(varDomNoInit3);
writeln();

writeln("const domain no init expr");
const constDomNoInit3: domain(1, int, false);
test(constDomNoInit3);
writeln();

writeln("domain of an array defined with range literal");
var arrWithRangeLiteral: [1..10] int;
test(arrWithRangeLiteral.domain);
writeln();

writeln("domain of an array defined with domain literal");
var arrWithDomainLiteral: [{1..10}] int;
test(arrWithDomainLiteral.domain);
writeln();

//var distSpace = {1..10};
//var distVarDom = distSpace dmapped Block(distSpace);
//var distVarArr: [distVarDom] int;

//writeln(distVarDom.definedConst);
//writeln(distVarArr);
