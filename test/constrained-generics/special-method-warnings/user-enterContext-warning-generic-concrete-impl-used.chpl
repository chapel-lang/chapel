record R {
    var x;
}

proc ref (R(int)).enterContext() ref: int {
    return this.x;
}

var r: R(int) = new R(1);
writeln(r.enterContext());
