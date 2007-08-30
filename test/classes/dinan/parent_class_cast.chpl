class P {
    def f return "";
}

class C: P {
    def f return "C";
}

class D: P {
    def f return "D";
}

// OK: var ps: [1..2] P = (C():P, D():P);
var ps: [1..2] P = (C(), D());

for i in ps do
    writeln(i.f);
