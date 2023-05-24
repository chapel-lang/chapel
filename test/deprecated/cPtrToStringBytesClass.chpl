use CTypes;

class Foo {}

var s = "hello",
    b = b"hello",
    c = new owned Foo();

var ps = c_ptrTo(s),
    pb = c_ptrTo(b),
    pc = c_ptrTo(c),
    cps = c_ptrToConst(s),
    cpb = c_ptrToConst(b),
    cpc = c_ptrToConst(c);
