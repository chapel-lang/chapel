use Sort;
use BitOps;

proc test(x) {
  var count = RadixSortHelp.radixSortClz(x);
  writef("%s %xu %i\n", x.type:string, x, count);
  assert(count == clz(x));
}

var x: uint(8) = 0x08;
test(x);
test(x:uint(16));
test(x:uint(32));
test(x:uint(64));

test(0xffffffff:uint(32));
test(0x7fffffff:uint(32));
test(0xffffffffffffffff:uint(64));
test(0x0fffffffffffffff:uint(64));
