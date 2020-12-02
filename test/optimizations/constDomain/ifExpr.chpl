config const flag = true;

writeln("var domain");
{
var dom = if flag then {1..10} else {0..9};
writeln(dom.definedConst);
writeln(dom._value.definedConst);
}

writeln("const domain");
{
const dom2 = if flag then {1..10} else {0..9};
writeln(dom2.definedConst);
writeln(dom2._value.definedConst);
}

config param paramFlag = true;
writeln("var domain");
{
var dom: if paramFlag then domain(1) else domain(2);
writeln(dom.definedConst);
writeln(dom._value.definedConst);
}

writeln("const domain");
{
const dom: if paramFlag then domain(1) else domain(2);
writeln(dom.definedConst);
writeln(dom._value.definedConst);
}

writeln("var domain (nested)");
{
var dom = if flag then {1..10} else if paramFlag then {10..100} else {0..9};
writeln(dom.definedConst);
writeln(dom._value.definedConst);
}

writeln("const domain (nested)");
{
const dom2 = if flag then {1..10} else if paramFlag then {10..100} else {0..9};
writeln(dom2.definedConst);
writeln(dom2._value.definedConst);
}
