proc testNumeric(type t, val) {
    var x = val : t;
    writeln(chpl__defaultHashWrapper(x));
}

testNumeric(int(64), 1);
testNumeric(int(32), 2);
testNumeric(int(16), 3);
testNumeric(int(8), 4);
testNumeric(uint(64), 1);
testNumeric(uint(32), 2);
testNumeric(uint(16), 3);
testNumeric(uint(8), 4);
testNumeric(complex(64), 1);
testNumeric(complex(128), 2);
testNumeric(real(32), 1);
testNumeric(real(64), 2);
testNumeric(imag(32), 1);
testNumeric(imag(64), 2);
