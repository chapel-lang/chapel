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

var d = new D();

// OK: d.p = new C(5):P;
d.p = new C(5);

writeln(d.p.f());
