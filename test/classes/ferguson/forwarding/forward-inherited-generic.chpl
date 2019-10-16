class Collection {
    type eltType;
    proc add(elt : eltType) : bool {
        halt("Bad");
    }
}

class Deque : Collection {
    override proc add(elt : eltType) : bool {
        writeln("Called");
        return true;
    }
}

record wrapper {
    type t;
    var instance : unmanaged Deque(t)?;

    inline proc _value {
        if instance == nil then
            instance = new unmanaged Deque(t);
        return instance!;
    }

    forwarding _value;

    proc deinit() {
        delete instance;
    }
}

var w : wrapper(int);
w.add(1);
