use BlockDist;

proc test(d) {
  var arr: [d] int;

  writeln("Using user-variable: ", d.definedConst);
  writeln("Using instance: ", d._value.definedConst);

  writeln("Array on domain: ", arr);
}

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

writeln("var distributed domain defined with var space");
var distVarSpace = {1..10};
var distVarDom = distVarSpace dmapped Block(distVarSpace);
test(distVarDom);
writeln();

writeln("var distributed domain defined with const space");
const distConstSpace = {1..10};
var distVarDomConstSpace = distConstSpace dmapped Block(distConstSpace);
test(distVarDomConstSpace);
writeln();

writeln("const distributed domain defined with var space");
const distConstDom = distVarSpace dmapped Block(distVarSpace);
test(distConstDom);
writeln();

writeln("const distributed domain defined with const space");
const distConstDomConstSpace = distConstSpace dmapped Block(distConstSpace);
test(distConstDomConstSpace);
writeln();

writeln("const domain copy-inited with var domain");
const constFromVar = varDom;
test(constFromVar);
writeln();

writeln("const domain copy-inited with var domain (explicit type)");
const constFromVarWithType: domain(1)  = varDom;
test(constFromVarWithType);
writeln();


