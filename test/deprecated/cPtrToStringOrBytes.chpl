use CTypes;

var s = "hello",
    b = b"hello";

var ps = c_ptrTo(s),
    pb = c_ptrTo(b),
    cps = c_ptrToConst(s),
    cpb = c_ptrToConst(b);
