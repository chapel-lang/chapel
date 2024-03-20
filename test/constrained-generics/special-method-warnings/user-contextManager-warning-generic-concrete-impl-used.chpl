record R {
    var x;
}

proc ref (R(int)).enterContext() ref: int {
    return this.x;
}
proc ref (R(int)).exitContext(in err: owned Error?) {}

var r: R(int) = new R(1);
writeln(r.enterContext());
r.exitContext(nil);
