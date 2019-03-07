class P {
    proc f() return "";
}

class C: P {
    override proc f() return "C";
}

class D: P {
    override proc f() return "D";
}

// OK: var ps: [1..2] P = (C():P, D():P);
var ps: [1..2] borrowed P = (new borrowed C(), new borrowed D());

for i in ps do
    writeln(i.f());
