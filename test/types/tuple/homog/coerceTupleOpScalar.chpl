// check that coercion applies for ops between scalars and tuples
proc testit(x) {
  writeln(x, ": ", x.type:string);
}

const t = (1.0, 2.0, 3.0);
testit(t + 1);
testit(t - 1);
testit(t * 2);
testit(t / 2);
testit(t ** 2);

const t2 = (1, 2, 3);
testit(t2 & 0x1:int(8));
testit(t2 | 0x1:uint(8));
testit(t2 ^ 7:int(16));
testit(t2 << 2:uint(16));
testit(t2 >> 2:int(32));
testit(t2 % 2:uint(32));
