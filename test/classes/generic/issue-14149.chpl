class Row {
    var valid: bool;
    var curRow: int;

    proc isValid(): bool {
        return valid;
    }
}

class CursorBase {
    iter fetchAll(): Row {}
    proc fetchOne(): Row {}
}

class Cursor: CursorBase {
    var curRow: int;
    var numRows: int;

    proc init() {
        this.curRow = 0;
        this.numRows = 4;
    }

    override iter fetchAll(): Row {
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
    var cursor = new owned Cursor();
    for row in cursor.fetchAll() {
        writeln((row.type :string, row));
    }
}
