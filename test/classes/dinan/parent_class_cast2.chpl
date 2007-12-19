class P {
    def f() return "";
}

class C: P {
    var n: int;
    def f() return "D";
}

class D {
    var p: P;
}

var d = D();

// OK: d.p = C(5):P;
d.p = C(5);

writeln(d.p.f());
