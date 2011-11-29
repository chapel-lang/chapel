class A {
    var k: int;
    proc A(k=5) {
        this.k = k;
    }
}

class B {
    var a: A;
    proc B(a = nil) {
        this.a = a;
    }
}

proc main() {
    const a1: A = new A();
    writeln(a1);
    const a2: A = new A(k=6);
    writeln(a2);
    const b = new B(a=a1);
    writeln(b);
}
