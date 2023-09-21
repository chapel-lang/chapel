class Parent : contextManager {
    var x: uint;

    proc enterContext() ref: uint {
        return x;
    }

    proc exitContext(in err: owned Error?) {}
}

class Child : Parent {
    var y: uint;

    override proc enterContext() ref: uint {
        return y;
    }
}

writeln(new Parent());
writeln(new Child());
