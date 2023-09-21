use Set;

record Outer {
    record Inner : hashable {
        proc hash(): uint {
            writeln("in hash of Inner record");
            return 0;
        }
    }

    proc getInnerSet() {
        var mySet: set(Inner);
        mySet.add(new Inner());
        return mySet;
    };
}

var outer: Outer;
writeln(outer.getInnerSet());
