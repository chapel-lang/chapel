var x: sync complex;
x.writeEF(1.0 + 3.0i);
assert(x.readFF() == 1.0 + 3.0i);
assert(x.readFE() == 1.0 + 3.0i);
