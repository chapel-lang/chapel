{
var d = new _domain(defaultDist, 1, int, false, false);
writeln(d.definedConst);
}

{
const d = new _domain(defaultDist, 1, int, false, true);
writeln(d.definedConst);
}
