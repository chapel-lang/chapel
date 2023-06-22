module M {

    record MyRecord {
        var x: int;
    }

    proc processArr(arr: [1..n] int) where n != 0 {
        writeln("processArr 1");
    }

    proc processArr(arr: []) where arr.elemType == int {
        writeln("processArr 2");
    }

    proc processDom(dom: domain) where dom.rank == 2 {
        writeln("processDom 1");
    }

    proc processRec(r: MyRecord) where r.x > 0 {
        writeln("processRec");
    }

    proc processRange(r: [1..n] int) where n > 0 {
        writeln("processRange 1");
    }

    proc processRange(r: range) where r.low > 1 {
        writeln("processRange 2");
    }

    operator + (a: int, b: int) where a > 0 {
        writeln("operator +");
    }
}

