use CPtr;

proc getAddr(obj) {
  return __primitive("_wide_get_addr", obj);
}
proc getAddrAndLocality(obj) : (locale, c_void_ptr) {
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
  var yinfo :(locale, c_void_ptr);
 
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

for b in buff do
  delete b.y;

writeln("Nothing to see here");
