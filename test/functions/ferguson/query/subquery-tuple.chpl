record C {
  var x;
}
record D {
  var z;
}

proc f(arg: (C(?t), D(?u))) {
  writeln("t:", t:string, " u:", u:string);
}


f( (new C(1), new D("hi")) );
