use BlockDist;

class C { var x: int = 0; }
record r { var x: int = 0; }
union u { var x: int; var y: real; }

proc modifyConstArgElement(const tup: (bool,)) {
  tup[0] = true;
}

proc modifyConstArgElement(const tup: (?t,)) where isIntegralType(t) {
  tup[0] = 64;
}

proc modifyConstArgElement(const tup: (?t,)) where isFloatType(t) {
  tup[0] = 64.0:t;
}

proc modifyConstArgElement(const tup: (?t,)) where isComplexType(t) {
  tup[0] = 64+0i;
}

proc modifyConstArgElement(const tup: (?t,)) where isRangeType(t) {
  tup[0] = 0..64;
}

proc modifyConstArgElement(const tup: (owned C?,)) {
  tup[0] = new owned C?(64);
}

proc modifyConstArgElement(const tup: (shared C?,)) {
  tup[0] = new shared C?(64);
}

proc modifyConstArgElement(const tup: (borrowed C?,)) {
  tup[0] = new borrowed C?(64);
}

proc modifyConstArgElement(const tup: (unmanaged C?,)) {
  tup[0] = new unmanaged C?(64);
}

proc modifyConstArgElement(const tup: (?t,)) where isAtomicType(t) {
  tup[0].write(64);
}

proc modifyConstArgElement(const tup: (?t,)) where isSingleType(t) {
  tup[0] = 64;
}

proc modifyConstArgElement(const tup: (?t,)) where isSyncType(t) {
  tup[0] = 64;
}

proc modifyConstArgElement(const tup: (?t,))
where isStringType(t) || isBytesType(t) {
  tup[0] = "foo";
}

proc modifyConstArgElement(const tup: (r,)) {
  tup[0].x = 64;
}

proc modifyConstArgElement(const tup: (u,)) {
  tup[0].x = 64;
}

proc modifyConstArgElement(const tup: (?t,)) where isDmapType(t) {
  // TODO: dmap
}

proc modifyConstArgElement(const tup: (?t,)) where isDomainType(t) {
  tup[0] = {0..64};
}

proc modifyConstArgElement(const tup: (?t,)) where isArrayType(t) {
  tup[0][0] = 64;
}

proc run(in elem) {
  var tup1 = (elem,);
  writeln(tup1);
  modifyConstArgElement(tup1);
  writeln(tup1);

  const tup2 = (elem,);
  writeln(tup2);
  modifyConstArgElement(tup2);
  writeln(tup2);
}

proc test() {

  // bool, OK
  run(false);

  // int, OK
  run(0:int);

  // uint, OK
  run(0:uint);

  // real, OK
  run(0:real);

  // imag, OK
  run(0:imag);

  // complex, OK
  run(0+0i);

  // range, OK
  run(0..0);

  // owned, OK
  run(new owned C?());

  // shared, OK
  run(new shared C?());

  // borrowed, OK
  run(new borrowed C?());

  // unmanaged, OK
  run(new unmanaged C?());
  
  // atomic, OK
  var atm: atomic int;
  run(atm);

  // TODO: single (cannot currently be stored in tuples)
  // var sng: single int;
  // run(sng);

  // TODO: sync (cannot currently be stored in tuples)
  // var snc: sync int;
  // run(snc);

  // string, OK
  run("foo":string);

  // bytes, OK
  run("foo":bytes);
 
  // record, OK
  run(new r());

  // union, OK
  run(new u());

  // TODO: dmap

  // domain, OK
  run({0..0});

  // array/formal=const, OK
  var arr: [0..0] int;
  run(arr);
}
test();

