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
var ps: [1..2] borrowed P = ((new owned C()).borrow(), (new owned D()).borrow());

for i in ps do
    writeln(i.f());
