class C {
  var i: int;
}

proc f( tup: (?t, borrowed C) ) {
  compilerWarning(tup(1).type:string); // should be owned C
  compilerWarning(tup(2).type:string); // should be borrowed C aka C
}

proc test1() {
  var a = new owned C(1);
  var b = new owned C(1);

  f( (a, b) );
}
test1();
