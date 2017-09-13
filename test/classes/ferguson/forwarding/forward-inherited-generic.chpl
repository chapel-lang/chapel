class Collection {
    type eltType;
    proc add(elt : eltType) : bool {
        halt("Bad");
    }
}

class Deque : Collection {
    proc add(elt : eltType) : bool {
        writeln("Called");
        return true;
    }
}

record wrapper {
    type t;

    inline proc _value { 
        return new Deque(t);
    }

    forwarding _value;
}

var w : wrapper(int);
w.add(1);
