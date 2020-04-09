var BigDom1 = {1..10},
    BigDom2 = {1..21, 1..22},
    BigDom3 = {1..31, 1..32, 1..33},
    BigDom4 = {1..41, 1..42, 1..43, 1..44};

var A1 : [BigDom1] int(8),
    A2 : [BigDom2] int(8),
    A3 : [BigDom3] int(8),
    A4 : [BigDom4] int(8);

var slice1 = {1..1},
    slice2 = {2..2, 2..3},
    slice3 = {3..3, 3..4, 3..5},
    slice4 = {4..4, 4..5, 4..6, 4..7};

writeln(BigDom1, " ", slice1, " ", BigDom1.localSlice(slice1), " ", (A1.localSlice(slice1)).domain);
writeln(BigDom2, " ", slice2, " ", BigDom2.localSlice(slice2), " ", (A2.localSlice(slice2)).domain);
writeln(BigDom3, " ", slice3, " ", BigDom3.localSlice(slice3), " ", (A3.localSlice(slice3)).domain);
writeln(BigDom4, " ", slice4, " ", BigDom4.localSlice(slice4), " ", (A4.localSlice(slice4)).domain);
