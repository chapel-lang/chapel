record C {
  var x;
}
record D {
  var z;
}

proc f(arg: (C(?t), D( (?u, C(?v))), (?w, ?x)) ...) {
  writeln(arg);
  writeln("t:", t:string, " u:", u:string, " v:", v:string,
          " w:", w:string, " x:", x:string);
}


proc test() {
  var i8:int(8) = 8;
  var u8:uint(8) = 16;
  var tup = (new C(1), new D( (1.0, new C("hi")) ), (i8, u8));
  f( tup, tup );
}

test();
