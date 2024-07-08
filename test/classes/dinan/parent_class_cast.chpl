class P {
    proc f() do return "";
}

class C: P {
    override proc f() do return "C";
}

class D: P {
    override proc f() do return "D";
}

// OK: var ps: [1..2] P = (C():P, D():P);

var a = new C();
var b = new D();

var ps: [1..2] borrowed P = (a.borrow(), b.borrow());

for i in ps do
    writeln(i.f());
