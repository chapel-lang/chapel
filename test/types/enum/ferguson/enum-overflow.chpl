enum myenum {
  a = 0x7fffffffffffffff,
  b,
  c = b + 2
}

proc writeint(x:int) {
  writef(" int %xi\n", x);
}
proc writeint(x:uint) {
  writef("uint %xu\n", x);
}

var t = myenum.a;
writeint(t);
writeint(myenum.b);
writeint(myenum.c);
