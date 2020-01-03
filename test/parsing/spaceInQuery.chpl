record GenericType {
  type t;
}

{ // no space versions
  proc regular1(a:?t) { writeln(t:string); }
  proc regular2(a: ?t) { writeln(t:string); }

  proc vararg1(a:?t...?n) { writeln(n, " ", t:string); }
  proc vararg2(a:?t... ?n) { writeln(n, " ", t:string); }
  proc vararg3(a:?t ...?n) { writeln(n, " ", t:string); }
  proc vararg4(a:?t ... ?n) { writeln(n, " ", t:string); }
  proc vararg5(a: ?t...?n) { writeln(n, " ", t:string); }
  proc vararg6(a: ?t... ?n) { writeln(n, " ", t:string); }
  proc vararg7(a: ?t ...?n) { writeln(n, " ", t:string); }
  proc vararg8(a: ?t ... ?n) { writeln(n, " ", t:string); }
  proc vararg9(a...?n) { writeln(n); }
  proc vararg10(a... ?n) { writeln(n); }

  proc arrElem1(a: [] ?t) { writeln(t:string); }
  proc arrElem2(a: []?t) { writeln(t:string); }
  proc arrElem3(a:[] ?t) { writeln(t:string); }
  proc arrElem4(a:[]?t) { writeln(t:string); }

  proc genType(a: GenericType(?t)) { writeln(t:string); }

  regular1(1);
  regular1(1.1);
  regular2(1);
  regular2(1.1);

  vararg1(1, 1);
  vararg2(1.1, 1.1);
  vararg3(1, 1);
  vararg4(1.1, 1.1);
  vararg5(1, 1);
  vararg6(1.1, 1.1);
  vararg7(1, 1);
  vararg8(1.1, 1.1);
  vararg9(1.1, 1.1);
  vararg10(1.1, 1.1);

  arrElem1([1, 1]);
  arrElem2([1.1, 1.1]);
  arrElem3([1, 1]);
  arrElem4([1.1, 1.1]);

  genType(new GenericType(int));
  genType(new GenericType(real));
}

{ //spaced versions -- they should all generate deprecation warnings
  proc regular1(a:? t) { writeln(t:string); }
  proc regular2(a: ? t) { writeln(t:string); }

  proc vararg1(a:? t...? n) { writeln(n, " ", t:string); }
  proc vararg2(a:? t... ? n) { writeln(n, " ", t:string); }
  proc vararg3(a:? t ...? n) { writeln(n, " ", t:string); }
  proc vararg4(a:? t ... ? n) { writeln(n, " ", t:string); }
  proc vararg5(a: ? t...? n) { writeln(n, " ", t:string); }
  proc vararg6(a: ? t... ? n) { writeln(n, " ", t:string); }
  proc vararg7(a: ? t ...? n) { writeln(n, " ", t:string); }
  proc vararg8(a: ? t ... ? n) { writeln(n, " ", t:string); }
  proc vararg9(a...? n) { writeln(n); }
  proc vararg10(a... ? n) { writeln(n); }

  proc arrElem1(a: [] ? t) { writeln(t:string); }
  proc arrElem2(a: []? t) { writeln(t:string); }
  proc arrElem3(a:[] ? t) { writeln(t:string); }
  proc arrElem4(a:[]? t) { writeln(t:string); }

  proc genType(a: GenericType(? t)) { writeln(t:string); }

  regular1(1);
  regular1(1.1);
  regular2(1);
  regular2(1.1);

  vararg1(1, 1);
  vararg2(1.1, 1.1);
  vararg3(1, 1);
  vararg4(1.1, 1.1);
  vararg5(1, 1);
  vararg6(1.1, 1.1);
  vararg7(1, 1);
  vararg8(1.1, 1.1);
  vararg9(1.1, 1.1);
  vararg10(1.1, 1.1);

  arrElem1([1, 1]);
  arrElem2([1.1, 1.1]);
  arrElem3([1, 1]);
  arrElem4([1.1, 1.1]);

  genType(new GenericType(int));
  genType(new GenericType(real));
}
