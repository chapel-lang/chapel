param entries:int = 4;

record table {
    param entries;
    forwarding var columns: [0..entries-1] shared column(?);

    proc init(param entries:int) {
        this.entries = entries;
	this.columns = [i in 0..entries-1] new shared column(entries);
	}
}

class column {
    param x:int;
}

var data = new table(entries);
writeln(data);
