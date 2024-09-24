class Row {
    var valid: bool;
    var curRow: int;

    proc isValid(): bool {
        return valid;
    }
}

class CursorBase {
    iter these() ref {
        use HaltWrappers only;
        HaltWrappers.pureVirtualMethodHalt();
    }

    iter fetchAll(): Row { // [[3]]
        use HaltWrappers only;
        HaltWrappers.pureVirtualMethodHalt();
    }

    proc fetchOne(): Row {
        use HaltWrappers only;
        HaltWrappers.pureVirtualMethodHalt();
    }
}

class Cursor: CursorBase {
    var curRow: int;
    var numRows: int;

    proc init() {
        this.curRow = 0;
        this.numRows = 4;
    }

    override iter these() {
        for row in this.fetchAll() {
            yield row;
        }
    }

    override iter fetchAll(): Row { // [[4]]
        var row: Row = this.fetchOne();
        while row.isValid() {
            yield row;
            row = this.fetchOne();
        }
    }

    override proc fetchOne(): Row {
        if this.curRow == this.numRows {
            return new shared Row(valid=false);
        }

        var row = new shared Row(valid=true, curRow=curRow);
        curRow += 1;
        return row;
    }
}

proc main() {
    var cursor = new Cursor();
    for row in cursor {
        writeln((row.type :string, row));
    }
}
