record r {
    var containedValue;

    proc doSomething() {
        return containedValue();
    }
}

var x = new r(proc() { return 42; });
writeln(x.doSomething());
