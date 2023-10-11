use Set;

class notAnInterface {}

union myUnion : hashable, notAnInterface {
    var x: int(64);
    var y: int(64);

    proc doSomething() {
        writeln("Doing something!");
    }

    proc hash(): uint {
        return 0;
    }

    // clearly a bad implementation, but the compiler (rightly) doesn't generate
    // == for us.
    operator ==(lhs: myUnion, rhs: myUnion) do return false;
}

var x: set(myUnion);
