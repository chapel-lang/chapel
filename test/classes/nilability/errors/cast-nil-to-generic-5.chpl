// #14148 - code from OP, with a fix:
// the return type is concrete


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

    override proc fetchone(): owned Row? { // [[2]]
        if (this.curRow == this.numRows) {
            return nil;
        }

        var row = new Row(curRow);
        return row;
    }
}

proc main() {
    var cursor = new owned Cursor();
    var res: owned Row? = cursor.fetchone();
    writeln(res);
}
