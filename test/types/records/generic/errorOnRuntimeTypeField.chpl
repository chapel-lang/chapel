    record MultiDual {
        type t;
        type d;
        var prim : t;
        var dual : d;
    }

    proc gradient(x : [?D] ?t) {
        type d = MultiDual(t, [D] t);
        var x0 : [D] d;
        forall i in D {
            var eps : [D] t = 0.0;
            eps[i] = 1.0;
            x0[i] = new d(x[i], eps); // this is line 15
        }
        return x0;
    }

    var x = [1.0, 2.0, 3.0];
    var a = gradient(x);
    writeln(a);
