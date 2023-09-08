use Set;

record R {
    var x;
}

proc (R(int)).hash(): uint {
    writeln("in hash");
    return this.x.hash();
}

var r: R(int) = new R(1);
writeln(r);
