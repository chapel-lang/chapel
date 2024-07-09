module M {

    use CTypes;

    record MyRecord {
        var x: int;
    }

    proc processArr(arr: [1..n] int, n: int) where n != 0 {
        writeln("processArr 1");
    }

    proc processArr(arr: []) where arr.elemType == int {
        writeln("processArr 2");
    }

    proc processDom(dom: domain(?)) where dom.rank == 2 {
        writeln("processDom 1");
    }

    proc processRec(r: MyRecord) where r.x > 0 {
        writeln("processRec");
    }

    proc processRange(r: [1..n] int, n: int) where n > 0 {
        writeln("processRange 1");
    }

    proc processRange(r: range) where r.low > 1 {
        writeln("processRange 2");
    }

    pragma "no where doc"
    proc processRangeNW(r: range) where r.low > 1 {
        writeln("processRange no where");
    }

    // From borrowed-in-where.chpl
    proc foo(type t) where isSubtype(t, int) {
        writeln("In foo where");
    }

    operator + (a: int, b: int) where a > 0 {
        writeln("operator +");
    }

    inline operator <<(x: ?t, y: _tuple) where isHomogeneousTuple(y) &&
                                               isCoercible(t, (y(0).type)) {
    }

    inline operator :(x: c_string, type t:c_ptrConst(?eltType))
    where eltType == c_char || eltType == int(8) || eltType == uint(8) {
        return __primitive("cast", t, x);
    }

    // From CMO_array.chpl
    iter these_example(param tag, followThis) ref
      where tag == iterKind.follower {
        yield followThis;
    }

    // From tree.chpl
    iter cubeiter ( param tag: iterKind, n: int, id: int=0, off: int = -1) : int
    where tag == iterKind.standalone {
        // if (id == 0) then writeln ("Running standalone parallel treeiter.");
        var offset = 1;
        if (off < 0) then
            while (offset*2+id < n) do offset = offset << 1;
        else
            offset = off;

        yield id;

        while (offset > 0) {
            if (id + offset < n ) then
                on Locales[id+offset] do
                for z in cubeiter(n, id+offset, offset >> 1, tag=iterKind.standalone)
                    do yield z;
                offset = offset >> 1;
        }
    }
}
