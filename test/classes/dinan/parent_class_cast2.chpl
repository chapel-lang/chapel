class P {
    proc f() return "";
}

class C: P {
    var n: int;
    override proc f() return "D";
}

class D {
    var p: unmanaged P;
}

var d = new unmanaged D();

// OK: d.p = new C(5):P;
d.p = new unmanaged C(5);

writeln(d.p.f());

delete d.p;
delete d;
