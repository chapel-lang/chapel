param c64one:complex(64) = 1.0;       testabs(c64one);
param c64negOne:complex(64) = -1.0i;  testabs(c64negOne);
param c128one:complex(128) = 1.0i;    testabs(c128one);
param c128negOne:complex(128) = -1.0; testabs(c128negOne);

proc testabs(param x) {
  param p = abs(x);
  assert(p == 1.0);
  // also check non-param version, just for good measure
  var v = x;
  assert(abs(v) == 1.0);
}
