// inspired by #14148
// differs from cast-nil-to-generic-1.chpl in casting nil to shared
// and creating a shared Row

class Row {
    var curRow: int;
}

class CursorBase {
    proc fetchone(): Row? { // [[1]]
        use HaltWrappers only;
        HaltWrappers.pureVirtualMethodHalt();
    }
}

class Cursor: CursorBase {
    var curRow: int;
    var numRows: int;

    proc init() {
        this.curRow = 0;
        this.numRows = 10;
    }

    override proc fetchone(): Row? { // [[2]]
        if (this.curRow == this.numRows) {
            return nil: shared;
        }

        var row = new shared Row(curRow);
        return row;
    }
}

proc main() {
    var cursor = new owned Cursor();
    var res: owned Row? = cursor.fetchone();
    writeln(res);
}
