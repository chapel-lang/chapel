class C {
    var x = 1;

    proc doSomething() {
        writeln(x);
    }

    proc init() {
        this.complete();
        this.doSomething();
    }
}
