// check that coercion applies for ops between scalars and tuples
proc testit(x) {
  writeln(x, ": ", x.type:string);
}

const t = (1.0, 2.0, 3.0);
testit(1 + t);
testit(1 - t);
testit(2 * t);
testit(6 / t);
testit(2 ** t);

const t2 = (1, 2, 3);
testit(0x1:int(8) & t2);
testit(0x1:uint(8) | t2);
testit(7:int(16) ^ t2);
testit(8:uint(16) << t2);
testit(8:int(32) >> t2);
testit(5:uint(32) % t2);
