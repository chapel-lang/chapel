proc getAddr(obj) {
  return __primitive("cast", uint(64), __primitive("_wide_get_addr", obj));
}
proc getAddrAndLocality(obj) : (locale, uint(64)) {
  return (obj.locale, getAddr(obj));
}
class A {
  const loc : locale;
 
  proc init() {
    loc = here.locale;
  }
}
 
record B {
  var y : unmanaged A?;
  var yinfo :(locale, uint(64));
 
  proc init() {

  }
  proc init(y : unmanaged A) {
    this.y = y;
    this.yinfo = getAddrAndLocality(y);
  }
}
 
 
config const N = 1024;
 
var buffDom = {1..N};
var buff : [buffDom] B;
for i in buffDom {
  var a = new unmanaged A();
  var rec = new B(a);
  buff[i] = rec;
}
for x in buff {
  assert(x.y!.loc == Locales[0]);
}
 
on Locales[numLocales-1] {
  const _buff = buff;
  for x in _buff {
    assert(x.y!.loc == Locales[0], "Bad object: ", x.y, ", locality info is ", getAddrAndLocality(x.y), " but should be ", x.yinfo);
  }
}
writeln("Nothing to see here");
