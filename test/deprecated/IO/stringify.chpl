use IO;

record r {
    var x, y: int;
}

writeln(stringify("r: ", new r(1, 2)));
