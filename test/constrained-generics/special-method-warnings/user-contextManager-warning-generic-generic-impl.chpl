record R {
    var x;
}

proc (R(integral)).enterContext() ref: this.x.type {
    return this.x;
}
proc (R(integral)).exitContext(in err: owned Error?) {}

var r: R(int) = new R(1);
writeln(r);
