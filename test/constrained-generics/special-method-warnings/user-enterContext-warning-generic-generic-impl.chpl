record R {
    var x;
}

proc (R(integral)).enterContext() ref: this.x.type {
    return this.x;
}

var r: R(int) = new R(1);
writeln(r);
