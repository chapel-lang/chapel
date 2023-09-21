use Set;

class C {
    var x: uint;

    override proc hash(): uint {
        writeln("in hash");
        return x.hash();
    }
}

writeln(new C());
