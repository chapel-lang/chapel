use BlockDist;

class C { var x: int = 0; }
record r { var x: int = 0; }
union u { var x: int; var y: real; }

proc modifyBlankArgElement(tup: (bool,)) {
  tup[0] = true;
}

proc modifyBlankArgElement(tup: (?t,)) where isIntegralType(t) {
  tup[0] = 64;
}

proc modifyBlankArgElement(tup: (?t,)) where isFloatType(t) {
  tup[0] = 64.0:t;
}

proc modifyBlankArgElement(tup: (?t,)) where isComplexType(t) {
  tup[0] = 64+0i;
}

proc modifyBlankArgElement(tup: (?t,)) where isRangeType(t) {
  tup[0] = 0..64;
}

proc modifyBlankArgElement(tup: (owned C?,)) {
  tup[0] = new owned C?(64);
}

proc modifyBlankArgElement(tup: (shared C?,)) {
  tup[0] = new shared C?(64);
}

proc modifyBlankArgElement(tup: (borrowed C?,)) {
  tup[0] = new borrowed C?(64);
}

proc modifyBlankArgElement(tup: (unmanaged C?,)) {
  tup[0] = new unmanaged C?(64);
}

proc modifyBlankArgElement(tup: (?t,)) where isAtomicType(t) {
  tup[0].write(64);
}

proc modifyBlankArgElement(tup: (?t,)) where isSingleType(t) {
  tup[0] = 64;
}

proc modifyBlankArgElement(tup: (?t,)) where isSyncType(t) {
  tup[0] = 64;
}

proc modifyBlankArgElement(tup: (?t,))
where isStringType(t) || isBytesType(t) {
  tup[0] = "foo";
}

proc modifyBlankArgElement(tup: (r,)) {
  tup[0].x = 64;
}

proc modifyBlankArgElement(tup: (u,)) {
  tup[0].x = 64;
}

proc modifyBlankArgElement(tup: (?t,)) where isDmapType(t) {
  // TODO: dmap
}

proc modifyBlankArgElement(tup: (?t,)) where isDomainType(t) {
  tup[0] = {0..64};
}

proc modifyBlankArgArrInferRef(tup: (?t,)) where isArrayType(t) {
  tup[0][0] = 64;
}

proc modifyBlankArgArrInferConst(tup: (?t,)) where isArrayType(t) {
  return;
}

proc run(in elem) {
  var tup1 = (elem,);
  writeln(tup1);
  modifyBlankArgElement(tup1);
  writeln(tup1);

  const tup2 = (elem,);
  writeln(tup2);
  modifyBlankArgElement(tup2);
  writeln(tup2);
}

proc runArrayRefFormal(arr) {
  var tup1 = (arr,);
  writeln(tup1);
  modifyBlankArgArrInferRef(tup1);
  writeln(tup1);

  const tup2 = (arr,);
  writeln(tup2);
  modifyBlankArgArrInferRef(tup2);
  writeln(tup2);
}

proc runArrayConstFormal(arr) {
  var tup1 = (arr,);
  writeln(tup1);
  modifyBlankArgArrInferConst(tup1);
  writeln(tup1);

  const tup2 = (arr,);
  writeln(tup2);
  modifyBlankArgArrInferConst(tup2);
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

  // array/formal=ref, OK
  var arr: [0..0] int;
  runArrayRefFormal(arr);
 
  // array/formal=const, OK
  runArrayConstFormal(arr);
}
test();

