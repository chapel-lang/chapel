record R {
    var x;
}

proc ref (R(int)).exitContext(in err: owned Error?) {}

var r: R(int) = new R(1);
r.exitContext(nil);
