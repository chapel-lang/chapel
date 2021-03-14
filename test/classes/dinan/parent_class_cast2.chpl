class P {
    proc f() return "";
}

class C: P {
    var n: int;
    override proc f() return "D";
}

class D {
    var p: unmanaged P?;
}

var d = new D();

d.p = new unmanaged C(5);

writeln(d.p!.f());

delete d.p;
