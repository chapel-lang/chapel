use Set;

record R {
    var x;
}

proc (R(int)).hash(): uint {
    writeln("in hash");
    return this.x;
}

var r: R(int) = new R(1);
writeln(r.hash());
